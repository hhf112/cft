#!/bin/bash
RELEASE="https://github.com/hhf112/cft/releases/download/stable-test"
INSTALL_DIR="/usr/local/cft"

echo "Thank you for installing cft!" && 
sudo mkdir "$INSTALL_DIR" && 
cd "$INSTALL_DIR" && 

# TEMPLATE
echo "downloading files ..." && 
curl -s -L -O "$RELEASE/template.cpp" &&

chmod u+r $PWD/template.cpp &&

# BIN
sudo mkdir "$INSTALL_DIR/bin" &&
cd $INSTALL_DIR/bin &&
echo "downloading files ... " && 
curl -s -L -O "$RELEASE/cft" &&
curl -s -L -O "$RELEASE/cpcmp" &&

echo "installing files ... " && 
chmod o+x $PWD/cft &&
chmod o+x $PWD/cpcmp &&

sudo ln -sf $PWD/cft $INSTALL_DIR/cft &&
sudo ln -sf $PWD/cpcmp $INSTALL_DIR/cpcmp &&

echo "
	  ██████╗███████╗████████╗███████╗███████╗████████╗███████╗██████╗      ██╗ ██╗ 
	 ██╔════╝██╔════╝╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝██╔════╝██╔══██╗    ████████╗
	 ██║     █████╗     ██║   █████╗  ███████╗   ██║   █████╗  ██████╔╝    ╚██╔═██╔╝
	 ██║     ██╔══╝     ██║   ██╔══╝  ╚════██║   ██║   ██╔══╝  ██╔══██╗    ████████╗
	 ╚██████╗██║        ██║   ███████╗███████║   ██║   ███████╗██║  ██║    ╚██╔═██╔╝
	  ╚═════╝╚═╝        ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝     ╚═╝ ╚═╝ 
	                                                                                
	 \t Installation completed successfully.
"
