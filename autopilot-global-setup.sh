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
sudo apt install uwsgi-plugin-python3

# redis
sudo apt install redis-server

# PostgreSQL
sudo apt update
sudo apt install postgresql postgresql-contrib
sudo -u postgres psql
\q
# create user
sudo -u postgres createuser --interactive

# set password for user
sudo -u postgres psql
ALTER USER user_name WITH PASSWORD 'new_password';
\q

#create db
sudo -u postgres psql
CREATE DATABASE test_db WITH TEMPLATE=template0 ENCODING='UTF8' LC_CTYPE='en_US.UTF-8' LC_COLLATE='en_US.UTF-8';
GRANT ALL privileges ON DATABASE test_db TO user_name;

# clone repos
git clone ssh://git@github.com/FCTools/autopilot-backend.git
git clone ssh://git@github.com/FCTools/autopilot-ts.git
git clone ssh://git@github.com/FCTools/autopilot-engine.git

# set up backend
cd autopilot/autopilot-backend
git checkout develop
python3.8 -m venv venv
. venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
deactivate
cd ..

# set up ts
cd autopilot-ts
git checkout develop
python3.8 -m venv venv
. venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
deactivate

cd ..
