cd "${TEST_DIR_DATA_GIT}"

root_dir="${TEST_DIR_DATA_GIT}"
origin_url="git@bitbucket.org:gochomugo/test-bh.git"
remote_url="git@bitbucket.org:gochomugo/test-bh2.git"
bad_url="test@example.com:test/bh-git.git"
file_merge_cause="merge-cause.txt"
file_new="new.txt"

function retry() {
    local max=5
    for i in $(seq 1 ${max})
    do
        ${*} && return 0 || {
            [[ "${i}" -eq "${max}" ]] && return 1
        }
    done
}
