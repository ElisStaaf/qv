install:
	@echo -e "\e[33m=================== Beginning Installation... ===================\e[0m"
	@echo
	@echo -e "\e[33mBuilding Executable...\e[0m"
	@$(CC) src/vine.c -o /usr/bin/vine -Wall -Wextra -pedantic -std=c99
	@echo -e "\e[32mSuccessfully built executable!\e[0m"
	@echo
	@echo -e "\e[32m=================== Installation Was A Success! ===================\e[0m\n"
