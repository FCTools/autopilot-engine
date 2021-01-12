cd ../..
sudo apt update
sudo apt-get update

# git, screen
sudo apt install git
sudo apt install screen

# python3.8 and extra packages installing
sudo apt update
sudo apt install software-properties-common
sudo add-apt-repository ppa:deadsnakes/ppa
sudo apt install python3.8
sudo apt install python3-venv
sudo apt install python3.8-venv
sudo apt install python3.8-dev

# git configuration
git config --global user.name "Autopilot Prod Server"
git config --global user.email "autopilot@fctools.com"
echo "SSH key generating: "
ssh-keygen
echo "Please add this key to github: "
cat .ssh/id_rsa.pub
read -p "Press enter to continue"
ssh -T git@github.com

# gcc, g++, cmake
sudo apt install build-essential
sudo apt update
sudo apt install snapd
sudo snap install cmake --classic

# uwsgi python3 module
sudo apt install uwsgi-plugin-python

# clone repos
git clone ssh://git@github.com/FCTools/autopilot-backend.git
git clone ssh://git@github.com/FCTools/autopilot-ts.git

# set up backend
cd autopilot/autopilot-backend
git checkout develop
python3.8 -m venv venv
. venv/bin/activate
pip install -r requirements.txt
deactivate
cd ..

# set up ts
cd autopilot-ts
git checkout develop
python3.8 -m venv venv
. venv/bin/activate
pip install -r requirements.txt
deactivate

cd ..
