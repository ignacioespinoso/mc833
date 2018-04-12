# Arquivo de Makefile
# use o Makefile para compilar o codigo corretamente

#  Giovani Nascimento Pereira - 168609
#	Ignacio Espinoso Ribeiro - 169767
#
#	MC833 - 2S2018
#	Unicamp

# -------- Como Usar ----------
#
# make test: Testar base de dados
# make server: Criar arquivo de servidor
# make client: Criar arquivo de client
# make run_client: Criar e rodar aquivo de client
# make run_server: Criar e rodar aquivo de server
# clean: Limpar todos os arquivos .out
#
# ------------------ plz use --

# Gerar arquivo de teste
test: Data.c teste.c
	gcc teste.c -o test.out
	./test.out
	rm -rf test.out

# Gerar arquivo do servidor
server: server.c Data.c timeManager.c libraries.h
	gcc server.c -o server.out

# Gerar arquivo do client
client: client.c timeManager.c libraries.h
	gcc client.c -o client.out

# Gerar todos os arquivos - Test only
all: client.c server.c Data.c timeManager.c libraries.h
	gcc client.c -o client.out
	gcc server.c -o server.out

# Gerar e rodar o servidor
run_server: server.c
	gcc server.c -o server.out
	./server.out

# Gerar e rodar o cliente
run_client: client.c
	gcc client.c -o client.out
	echo "run the command ./client.out xxx.x.x.x - with the server IP"

# Rodar arquivo de client com conexao local
run_local_client: client.c
	gcc client.c -o client.out
	./client.out 127.0.0.1

client_local_test: client.c timeManager.c libraries.h
	gcc client.c -o client.out
	./client.out 127.0.0.1 TEST

# Limpar arquivos de server
clean_server: server.out
	rm -rf server.out

# Limpar arquivos de client
clean_client: client.out
	rm -rf client.out

# Limpar arquivo de teste
clean_test: test.out
	rm -rf test.out

# Limpar arquivos de log
clean_log:
	rm -rf clientTimeLog.txt
	rm -rf serverTimeLog.txt

# Limpar todos os arquivo
clean:
	rm -rf *.out
	rm -rf clientTimeLog.txt
	rm -rf serverTimeLog.txt
