current_dir=`pwd`
service down cv
cp -r include /usr/src/
cp -r lib /usr/src/
cp -r servers /usr/src/
cd /usr/src/servers/cv
make
make install
cd /usr/src/lib/
make
make install

cd $current_dir
service up /usr/sbin/cv
