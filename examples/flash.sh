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

# The Bash script code that uses `optarg` to get the command line arguments comes from:
# https://www.baeldung.com/linux/use-command-line-arguments-in-bash-script#flags
#
while getopts b: arg
do
  case "${arg}" in
    b) BORDER_ROUTER_PORT=${OPTARG};;
  esac
done

function get_cipher_flag() {
  cat $1/sdkconfig | grep CONFIG_THREAD_ASCON_CIPHER_SUITE | tail -c 2 | head -1
}

function flag_to_cipher_string () {
  case $1 in
    0) echo "AES" ;;
    1) echo "No Encryption" ;;
    2) echo "ASCON-128a (ESP32 Optimized)" ;;
    3) echo "ASCON-128a (Reference)" ;;
    4) echo "ASCON-128a (LibAscon)" ;;
    5) echo "ASCON-128 (LibAscon)" ;;
  esac
}

function rcp_auto_update_flag() {
  cat $1/sdkconfig | grep CONFIG_AUTO_UPDATE_RCP | tail -c 2 | head -1
  
}

. $HOME/esp/esp-idf/export.sh > /dev/null

rcp_path="$IDF_PATH/examples/openthread/ot_rcp"
border_router_path="$HOME/Desktop/Repositories/br_throughput/examples/basic_thread_border_router"

rcp_cipher_flag=$(get_cipher_flag $rcp_path)
border_router_cipher_flag=$(get_cipher_flag $border_router_path)

# Make sure RCP Auto Update is enabled on the Thread Border Router. If it is not,
# then the built RCP will not be automatically flashed onto the Border Router.
#
rcp_auto_update_flag=$(cat $border_router_path/sdkconfig | grep CONFIG_AUTO_UPDATE_RCP | tail -c 2 | head -1)
if [[ "$rcp_auto_update_flag" != "y" ]]
then
  echo "ERROR: $(cat $border_router_path/sdkconfig | grep CONFIG_AUTO_UPDATE_RCP)"
  exit 1
fi

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
  echo "ERROR: RCP and Border Router have an encryption algorithm mismatch!"
  echo "RCP Encryption Flag:           $(flag_to_cipher_string $rcp_cipher_flag)"
  echo "Border Router Encryption Flag: $(flag_to_cipher_string $border_router_cipher_flag)"
  exit 1
fi