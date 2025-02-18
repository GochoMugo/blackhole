/**
 * #!gochomugo
 * Numbers in Counters
 */
#include "main.h"

/**
 * Create, if allowed, a counter file
 *
 * @param out The newly-created file
 * @param counters_path Path to the directory holding the counters
 * @param event The event in consideration
 * @param force_create Whether to forcibly create the file, if non-existent
 * @return 0 or error code
 */
int bh_counter__file(FILE **out, const char *counters_path, const char *event,
                     int force_create) {
  int ret_code = 0;
  char *path = NULL;
  FILE *file = NULL;

  return_err_ext(contra_path_join(&path, counters_path, event),
                 BH_DAEMONERR_PATHRESLV);

  file = fopen(path, "r+");
  /* if we have successfully opened the file, we return ok */
  if (NULL != file) {
    *out = file;
    return_ok(0);
  }

  /* if the error is not due to the file being non-existent, we return
   * with an error */
  if (ENOENT != errno) {
    return_err_now(BH_COUNTERERR_FOPEN);
  }

  /* if we are not required to create the file, we return with an error */
  if (false == force_create) {
    return_err_now(BH_COUNTERERR_ENOENT);
  }

  return_err_ext(mkdirp(counters_path, S_IRWXU), BH_COUNTERERR_MKDIR);

  file = fopen(path, "w+");
  /* if we can not create the file, we return with an error */
  if (NULL == file) {
    return_err_now(BH_COUNTERERR_FOPEN);
  }

  return_err_ext(fprintf(file, "%d", 0), BH_COUNTERERR_FWRITE);

  return_err_ext(fseek(file, 0, SEEK_SET), BH_COUNTERERR_FREAD);

  *out = file;

on_error:
  if (NULL != file)
    fclose(file);
cleanup:
  if (NULL != path)
    free(path);
  return ret_code;
}

/**
 * Read the integer stored in the counter file
 *
 * @param out The stored integer
 * @param file The file pointer for the counter file
 * @return 0 or error code
 */
int bh_counter__get(int *out, FILE *file) {
  int ret_code = 0;
  int count = -1;

  ret_code = fscanf(file, "%d", &count);
  /* if the file is of zero-length */
  if (EOF == ret_code) {
    count = 0;
    ret_code = 0;
  }
  return_err_ext(ret_code, BH_COUNTERERR_FREAD);

  return_err_ext(fseek(file, 0, SEEK_SET), BH_COUNTERERR_FREAD);

  /* if the count is still -1, we know we have an error
   * in reading the file */
  if (-1 == count)
    return_err_now(BH_COUNTERERR_FREAD); // TODO

  *out = count;

on_error:
cleanup:
  return ret_code;
}

int bh_counter_get(int *out, const char *counters_path, const char *event) {
  int ret_code = 0;
  FILE *file = NULL;

  ret_code = bh_counter__file(&file, counters_path, event, false);
  if (0 > ret_code) {
    /* we do have an error, but it is due to the counter missing
     * we assume it has not been created yet, so value = 0
     * and return ok */
    if (BH_COUNTERERR_ENOENT == ret_code) {
      *out = 0;
      return_ok(0);
    }
    return_err(ret_code);
  }

  return_err(bh_counter__get(out, file));

on_error:
cleanup:
  if (NULL != file)
    fclose(file);
  return ret_code;
}

int bh_counter_increment(int *out, const char *counters_path,
                         const char *event) {
  int ret_code = 0;
  int count = 0;
  FILE *file = NULL;

  return_err(bh_counter__file(&file, counters_path, event, true));

  return_err(bh_counter__get(&count, file));

  return_err_ext(ftruncate(fileno(file), 0), BH_COUNTERERR_TRUNCATE);

  count++;

  return_err_ext(fprintf(file, "%d", count), BH_COUNTERERR_FWRITE);

  *out = count;

on_error:
cleanup:
  if (NULL != file)
    fclose(file);
  return ret_code;
}

int bh_counter_reset(const char *counters_path, const char *event) {
  int ret_code = 0;
  char *path = NULL;

  return_err_ext(contra_path_join(&path, counters_path, event),
                 BH_DAEMONERR_PATHRESLV);

  ret_code = unlink(path);
  /* if the error is due to the file being non-existent, we ignore it */
  if (ENOENT == errno)
    ret_code = 0;
  return_err_ext(ret_code, BH_COUNTERERR_UNLINK);

on_error:
cleanup:
  if (NULL != path)
    free(path);
  return ret_code;
}

int bh_counter_tick(int *tick, const char *path, const char *event,
                    int intervals) {
  int ret_code = 0;
  int count = 0;

  return_err(bh_counter_increment(&count, path, event));

  *tick = 0 == ((count - 1) % intervals) ? true : false;

on_error:
cleanup:
  return ret_code;
}
