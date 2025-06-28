#!/bin/bash
echo "Thank you for installing cft!"  
sudo mkdir /usr/local/cft  
cd /usr/local/cft  

echo "downloading files ..."  
curl -s -L -O https://github.com/hhf112/cft/releases/download/stable-test/template.cpp 

chmod u+r $PWD/template.cpp 

echo "installing files ... "  
sudo mkdir /usr/local/cft/bin 
cd /usr/local/cft/bin 

echo "downloading files ... "  
curl -s -L -O https://github.com/hhf112/cft/releases/download/stable-test/cft 
curl -s -L -O https://github.com/hhf112/cft/releases/download/stable-test/cpcmp 

echo "installing files ... "  
chmod o+x $PWD/cft 
chmod o+x $PWD/cpcmp 

sudo ln -sf $PWD/cft /usr/local/bin/cft 
sudo ln -sf $PWD/cpcmp /usr/local/bin/cpcmp 

echo "
	  ██████╗███████╗████████╗███████╗███████╗████████╗███████╗██████╗      ██╗ ██╗ 
	 ██╔════╝██╔════╝╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝██╔════╝██╔══██╗    ████████╗
	 ██║     █████╗     ██║   █████╗  ███████╗   ██║   █████╗  ██████╔╝    ╚██╔═██╔╝
	 ██║     ██╔══╝     ██║   ██╔══╝  ╚════██║   ██║   ██╔══╝  ██╔══██╗    ████████╗
	 ╚██████╗██║        ██║   ███████╗███████║   ██║   ███████╗██║  ██║    ╚██╔═██╔╝
	  ╚═════╝╚═╝        ╚═╝   ╚══════╝╚══════╝   ╚═╝   ╚══════╝╚═╝  ╚═╝     ╚═╝ ╚═╝ 
	                                                                                
	 \t Installation completed successfully.
"
