git config filter.password.clean "sed -e 's/-PASSWORD-/-PASSWORD-/' -e 's/-SSID-/-SSID-/'"

git config filter.password.smudge "sed -e 's/-PASSWORD-/-PASSWORD-/' -e 's/-SSID-/-SSID-/'"

cat > .git/info/attributes << EOF
** filter=password
EOF
