# Arquivo de Makefile
# use o Makefile para compilar o codigo corretamente

#  Giovani Nascimento Pereira - 168609
#	Ignacio Espinoso Ribeiro - 169767
#
#	MC833 - 2S2018
#	Unicamp

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
	./client.out

# Limpar arquivos de log
clean_log:
	rm -rf clientTimeLog.txt
	rm -rf serverTimeLog.txt

# Limpar todos os arquivo
clean:
	rm -rf *.out
	rm -rf clientTimeLog.txt
	rm -rf serverTimeLog.txt
