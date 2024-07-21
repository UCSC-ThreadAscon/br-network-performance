. $HOME/esp/esp-idf/export.sh > /dev/null

rcp_path="$IDF_PATH/examples/openthread/ot_rcp"
border_router_path="./basic_thread_border_router"

function get_cipher_flag() {
  cat $1/sdkconfig | grep CONFIG_THREAD_ASCON_CIPHER_SUITE | tail -c 2 | head -1
}

# Compare the encryption algorithm that is set in the sdkconfigs
# of the RCP and Border Router. If they are different, throw an error.
#
rcp_cipher_flag=$(get_cipher_flag $rcp_path)
border_router_cipher_flag=$(get_cipher_flag $border_router_path)

echo "RCP Encryption Flag:           ${rcp_cipher_flag}"
echo "Border Router Encryption Flag: ${border_router_cipher_flag}"