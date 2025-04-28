    TEST_PROG="./jdis"
    REF_PROG="./jdis_prof"

    GREEN='\033[0;32m'
    RED='\033[0;31m'
    NO_COLOR='\033[0m'

    declare -a TESTS=(
        "./textes/x0.txt ./textes/x1.txt"
        "./textes/x1.txt ./textes/x2.txt"
        "./textes/x2.txt ./textes/x0.txt"
        "./textes/x0.txt ./textes/x1.txt ./textes/x2.txt"
        "-i2 ./textes/x0.txt ./textes/x1.txt ./textes/x2.txt"
        "-g -i2 ./textes/x0.txt ./textes/x1.txt ./textes/x2.txt"
        "-g ./textes/toto0.txt ./textes/toto1.txt ./textes/toto2.txt ./textes/toto3.txt"
        "./textes/toto0.txt ./textes/toto1.txt ./textes/toto2.txt ./textes/toto3.txt"
        "-g -p ./textes/toto0.txt ./textes/toto1.txt ./textes/toto2.txt ./textes/toto3.txt"
        "-p ./textes/toto0.txt ./textes/toto1.txt ./textes/toto2.txt ./textes/toto3.txt"
        "./textes/toto0.txt ./textes/toto0.txt"
        "./textes/toto0.txt ./textes/lesmiserables.txt"
        "-p ./textes/toto0.txt ./textes/lesmiserables.txt"
        "./textes/lesmiserables.txt ./textes/sssrmllieeeba.txt ./textes/abeeeillmrsss.txt"
        "./textes/fr_.txt ./textes/domjuan.txt ./textes/tartuffe.txt"
    )

    for args in "${TESTS[@]}"; do
        output_test=$("$TEST_PROG" $args 2>/dev/null)
        output_ref=$("$REF_PROG" $args 2>/dev/null)

        if diff <(echo "$output_test") <(echo "$output_ref") > /dev/null; then
            echo -e "${GREEN}SUCCESS: $args ${NO_COLOR}"
        else
            echo -e "${RED} FAILED : $args${NO_COLOR}"
            diff <(echo "$output_test") <(echo "$output_ref")
        fi
    done
