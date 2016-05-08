# Remove WiFi SSID and password
#git config filter.password.clean "sed -e 's/[RealPassword]/[PasswordPlaceHolder]/' -e 's/[RealSSID]/[SSIDPlaceHolder]/'"
git config filter.password.clean "sed -e 's/-PASSWORD-/-PASSWORD-/' -e 's/-SSID-/-SSID-/'"

# Restore WiFi SSID and password
#git config filter.password.smudge "sed -e 's/[PasswordPlaceHolder]/[RealPassword]/' -e 's/[SSIDPlaceHolder]/[RealSSID]/'"

cat > .git/info/attributes << EOF
** filter=password
EOF
