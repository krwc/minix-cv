current_dir=`pwd`
service down cv
cp -r include /usr/src/
cp -r lib /usr/src/
cp -r servers /usr/src/
cd /usr/src/servers/cv
make clean
make
make install
service up /usr/sbin/cv 
