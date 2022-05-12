NAME = webserv

FLAGS = -Wall -Wextra -Werror -std=c++98 -I includes

CXX = @c++ $(FLAGS)

SRCS_DIR = srcs

CGI_DIR = $(SRCS_DIR)/cgi

CONFIG_DIR = $(SRCS_DIR)/config

SRCS =	$(CGI_DIR)/cgi.cpp\
		$(CGI_DIR)/cgi_env.cpp\
		$(CONFIG_DIR)/config.cpp\
		$(CONFIG_DIR)/server_config.cpp\
		$(CONFIG_DIR)/location_config.cpp\
		$(SRCS_DIR)/main.cpp\
		$(SRCS_DIR)/parsing.cpp\
		$(SRCS_DIR)/utils.cpp\
		$(SRCS_DIR)/request.cpp\
		$(SRCS_DIR)/autoindex.cpp\
		$(SRCS_DIR)/mime_select.cpp

OBJ = $(SRCS:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJ)
			$(CXX) $(OBJ) -o $(NAME)
			@echo "\033[32mCOMPILATION OK\033[0m"
			
clean:
		@rm -rf $(OBJ)
		@echo "\033[36mCLEAN OK\033[0m"

fclean: clean
		@rm -rf $(NAME)
		@echo "\033[36mFCLEAN OK\033[0m"

re: fclean all

.PHONY: all fclean clean re