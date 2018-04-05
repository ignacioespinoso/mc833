# Arquivo de Makefile
# use o Makefile para compilar o codigo corretamente

#  Giovani Nascimento Pereira - 168609
#	Ignacio Espinoso Ribeiro - 
#
#	MC833 - 2S2018
#	Unicamp

# -------- Como Usar ----------
#
# make test: Testar base de dados
# (TODO) make server: Criar arquivo de servidor
# (TODO) make client: Criar arquivo de client
# (TODO) make run_client: Criar e rodar aquivo de client
# (TODO) make run_server: Criar e rodar aquivo de server
# clean: Limpar todos os arquivos .out
#
# ----------------- plz use ---

# Gerar arquivo de teste
test: Data.c teste.c
	gcc teste.c -o test.out
	./test.out
	rm -rf test.out

# Limpar arquivo de teste
clean_test: test.out
	rm -rf test.out

# Limpar todos os arquivo
clean:
	rm -rf *.out