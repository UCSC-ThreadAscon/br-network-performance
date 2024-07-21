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
  cd $border_router_path && idf.py build
  cd -
else
  echo "ERROR: RCP and Border Router having an encryption algorithm mismatch!"
  echo "RCP Encryption Flag:           ${rcp_cipher_flag}"
  echo "Border Router Encryption Flag: ${border_router_cipher_flag}"
  exit 1
fi