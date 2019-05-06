cd ../../../
make -f Makefile_client_server clean
make -f Makefile_client_server	
cd pruebas/03paralelo-sin-valgrind/revoke-invalid-subject
cp ../../../client ../../../server ../revoke-invalid-subject

chmod +x run.sh free_port.sh;./run.sh 127.0.0.1 $(./free_port.sh 12001) no-valgrind multiple

echo Certificate Client0
diff "client0.out" "client0_.out"
echo Index
diff index.txt index_.txt
echo ServerError
diff __server_stderr__ __server_stderr___ 
echo ServerOut
diff __server_stdout__ __server_stdout___
