#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: sudo ./keypadlistener.sh <username>"
    exit 1
fi

DEVICE="/dev/virtual_keypad"
BYTES_TO_READ=4
XDG_OPEN="/usr/bin/xdg-open"
USERNAME=$1

echo $USERNAME

if [[ ! -e "$DEVICE" ]]; then
    echo "Device $DEVICE not found!"
    exit 1
fi

exec 3<"$DEVICE"

while true; do
    if read -u 3 -N $BYTES_TO_READ cmd; then
        case "$cmd" in
            "PAY!")
                # Opens the scanner image
                echo "Received command: PAY!"
                echo "Opening upiscan.jpg..."
                sudo feh upiscan.jpg
                ;;
            "FAQs")
                # Opens the FAQ document
                echo "Received command: FAQs"
                echo "Opening faqs.pdf..."
                sudo -u $USERNAME $XDG_OPEN faq.pdf
                ;;
            "HELP")
                # Opens the webpage for help
                echo "Received command: HELP"
                echo "Opening www.google.com..."
                sudo -u $USERNAME $XDG_OPEN "http://www.google.com"
                ;;
        esac
    fi
done
