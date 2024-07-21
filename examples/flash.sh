# This Bash script does the following:
#
#   1. Sets BOTH the RCP and the Border Router to the encryption algorithm specified in
#      the command line arguments.
#
#      Options are: AES, ASCON-128a, ASCON-128, No Encryption
#
#   2. Checks whether the RCP and Border Router are using the same encryption algorithm.
#      If not, the script throws an error.
#
#   3. If both the RCP and Border Router are using the same encryption algorithm,
#      the script will build and flash the program onto the border router to the USB Modem
#      specified in the command line arguments.
#

# The Bash script code that uses `getopt` to get the command line arguments all comes from:
# https://dustymabe.com/2013/05/17/easy-getopt-for-a-bash-script/
#
options=$(getopt --long rcp_port: -- "$@" border_router_port: -- "$@")
[ $? -eq 0 ] || { 
    echo "Incorrect options provided"
    exit 1
}

eval set -- "$options"
while true; do
  case "$1" in
  --rcp_port)
    shift
    RCP_PORT=$1
    ;;
  --border_router_port)
    shift
    BORDER_ROUTER_PORT=$1
    ;;
  --)
      shift
      break
      ;;
  esac
  shift
done

echo "Border Router Port: $BORDER_ROUTER_PORT"
echo "RCP Port: $RCP_PORT"

. $HOME/esp/esp-idf/export.sh > /dev/null

rcp_path="$IDF_PATH/examples/openthread/ot_rcp"
border_router_path="./basic_thread_border_router"

function get_cipher_flag() {
  cat $1/sdkconfig | grep CONFIG_THREAD_ASCON_CIPHER_SUITE | tail -c 2 | head -1
}

rcp_cipher_flag=$(get_cipher_flag $rcp_path)
border_router_cipher_flag=$(get_cipher_flag $border_router_path)

# Compare the encryption algorithm that is set in the sdkconfigs
# of the RCP and Border Router. If they are different, throw an error.
#
if [[ "$rcp_cipher_flag" == "$border_router_cipher_flag" ]]
then
  cd $rcp_path && idf.py build
  cd -
  cd $border_router_path && idf.py build flash monitor --port $BORDER_ROUTER_PORT
  cd -
else
  echo "ERROR: RCP and Border Router having an encryption algorithm mismatch!"
  echo "RCP Encryption Flag:           ${rcp_cipher_flag}"
  echo "Border Router Encryption Flag: ${border_router_cipher_flag}"
  exit 1
fi