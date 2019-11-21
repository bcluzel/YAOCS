echo '--------------CLIENT-----------------'
cd client
make 
echo '--------------SERVER-----------------'
cd ../server
make
cd ..
rm YAOCS_client
rm YAOCS_server
cp ./client/bin/YAOCS_client YAOCS_client
cp ./server/bin/YAOCS_server YAOCS_server

