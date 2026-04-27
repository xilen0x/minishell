# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jocuni-p <jocuni-p@student.42barcelona.com +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/06 17:59:33 by castorga          #+#    #+#              #
#    Updated: 2024/06/10 11:33:10 by jocuni-p         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME =	minishell

# Compiler
CC := cc

# Compiler flags
FLAGS := -Wall -Werror -Wextra -MMD -g

# Remove
RM 	:=	rm -rf

# Makefile
MKF :=	Makefile

# Root Folders
SRC_ROOT := src/
DEP_ROOT := .dep/
OBJ_ROOT := .objs/
INC_ROOT := include/
LIB_ROOT := lib/

INC_DIRS += ${INC_ROOT}

# Lib readline
READLINE_MK_ROOT := ${LIB_ROOT}readline/Makefile
READLINE_ROOT := ${LIB_ROOT}readline/
READLINE := ${READLINE_ROOT}libreadline.a ${READLINE_ROOT}libhistory.a

# Libft
LIBFT_ROOT := ${LIB_ROOT}libft/
LIBFT_INC := $(LIBFT_ROOT)
LIBFT := $(LIBFT_ROOT)libft.a

INC_DIRS += ${LIBFT_INC}
LIBS += -L${LIBFT_ROOT} -lft

INC_DIRS += ${READLINE_ROOT}
LIBS += -L${READLINE_ROOT} -ltermcap

# Source Files
################################################################################

FILES =	main.c \
								minishell.c \
								arr2d/add_one_arr2d.c \
								arr2d/dup_arr2d.c \
								arr2d/free_arr2d.c \
								arr2d/rm_one_arr2d.c \
								arr2d/size_arr2d.c \
								builtins/builtin_cd.c \
								builtins/builtin_echo.c \
								builtins/builtin_env.c \
								builtins/builtin_exit.c \
								builtins/builtin_export.c \
								builtins/builtin_pwd.c \
								builtins/builtin_unset.c \
								builtins/builtins.c \
								builtins/utils_pwd/oldpwd.c \
								builtins/utils_export/var_exists.c \
								builtins/utils_export/checks.c \
								builtins/utils_cd/utils_cd.c \
								builtins/utils_cd/utils_cd_2.c \
								cmd/cmd_add_back.c \
								cmd/cmd_free.c \
								cmd/cmd_last.c \
								cmd/cmd_new_node.c \
								cmd/cmd_size.c \
								env_list/env_cleaner_list.c \
								env_list/env_delone_lst.c \
								env_list/env_init_list.c \
								env_list/env_lstadd_back.c \
								env_list/env_lstlast.c \
								env_list/env_lstnew.c \
								executor/executor.c \
								executor/redirections.c \
								executor/path.c \
								executor/heredoc.c \
								exit_status/error_msgs.c \
								exit_status/error_msgs2.c \
								exit_status/get_exit_status_len.c \
								exit_status/get_exit_status_val.c \
								exit_status/set_exit_status.c \
								expander/expand_quote_rm.c \
								expander/get_dollar_builder.c \
								expander/get_dollar_len.c \
								expander/get_env_key.c \
								expander/get_env_val.c \
								expander/get_len_and_free.c \
								expander/handle_dollar_invalid_syntax.c \
								expander/handle_dollar_question.c \
								expander/handle_quote_after_dollar.c \
								expander/handle_quote_builder.c \
								expander/handle_quote_len.c \
								expander/init_xpdr_except_result.c \
								expander/init_xpdr.c \
								expander/new_tok_builder.c \
								expander/new_tok_len.c \
								expander/should_expand.c \
								parser/commands_counter.c \
								parser/commands_creator.c \
								parser/commands_filler.c \
								parser/handle_error.c \
								parser/is_operator.c \
								parser/is_redirection.c \
								parser/parser.c \
								parser/syntax_check_1.c \
								parser/syntax_check_2.c \
								prints/print_arr2d.c \
								prints/print_cmd_para_executor.c \
								prints/print_cmd.c \
								prints/print_envlist.c \
								prints/print_redir.c \
								prints/print_tok.c \
								redirs/redir_add_back.c \
								redirs/redir_free.c \
								redirs/redir_last.c \
								redirs/redir_new_node.c \
								redirs/redir_size.c \
								tokenizer/tokenizer.c \
								tokenizer/init_operator_type.c \
								tokenizer/tok_len.c \
								tokenizer/init_word_str.c \
								tokens/tok_add_back.c \
								tokens/tok_free.c \
								tokens/tok_last.c \
								tokens/tok_new_node.c \
								tokens/tok_size.c \
								utils/p_malloc.c \
								utils/utils1.c \
								utils/utils3.c \
								utils/signals_utils.c \
								utils/wellcome_msg.c \
								init_exe.c \
								signals.c

SRC 	:= $(addprefix $(SRC_ROOT), $(FILES))
OBJS 	:= $(addprefix $(OBJ_ROOT), $(FILES:.c=.o))
DEPS 	:= $(addprefix $(DEP_ROOT), $(FILES:.c=.d))
INCS 	:= $(addprefix -I, $(INC_DIRS))


# Colors
################################################################################

DEF_COLOR =		\033[0;39m
DARK_GREEN 	=	\033[1m\033[38;2;75;179;82m
GREEN 		=	\033[0;32m

# Rules
################################################################################

all: $(NAME)

# 1. Configurar readline si no lo está (solo si no existe su Makefile)
$(READLINE_MK_ROOT):
	cd ./${READLINE_ROOT} && ./configure

# 2. Compilar readline (depende del configure; force garantiza re-evaluación)
$(READLINE): $(READLINE_MK_ROOT) force
	@echo "$(DARK_GREEN)GNU READLINE 8.2 COMPILING... $(DEF_COLOR)"
	@$(MAKE) -sC $(READLINE_ROOT)

# 3. Compilar libft (force garantiza re-evaluación si cambia internamente)
$(LIBFT): force
	@echo "$(DARK_GREEN)LIBFT COMPILING... $(DEF_COLOR)"
	@$(MAKE) -C $(LIBFT_ROOT)

# 4. Linkado del ejecutable (Make resuelve el orden de libft y readline)
$(NAME): $(OBJS) $(LIBFT) $(READLINE)
	@echo "$(DARK_GREEN)MINISHELL LINKING... $(DEF_COLOR)"
	@$(CC) $(FLAGS) $(OBJS) $(READLINE) $(LIBS) -o $(NAME)
	@echo "$(GREEN)▶  MINISHELL BUILD COMPLETED!$(DEF_COLOR)"
	@echo "$(DARK_GREEN)-->	Now you can run ./minishell$(DEF_COLOR)"

$(OBJ_ROOT)%.o: $(SRC_ROOT)%.c $(MKF)
	@mkdir -p $(dir $@) $(dir $(subst $(OBJ_ROOT), $(DEP_ROOT), $@))
	@echo "▶ Compiling minishell file: <$(notdir $<)>"
	@$(CC) $(FLAGS) $(INCS) -c $< -o $@
	@mv $(patsubst %.o, %.d, $@) $(dir $(subst $(OBJ_ROOT), $(DEP_ROOT), $@))

-include $(DEPS)

clean:
	@$(RM) $(OBJ_ROOT)
	@$(RM) $(DEP_ROOT)
	@$(MAKE) clean -C $(LIBFT_ROOT)

fclean: clean
	@$(RM) $(NAME)
	@$(RM) $(LIBFT)
	@if [ -f $(READLINE_MK_ROOT) ]; then $(MAKE) distclean -sC $(READLINE_ROOT); fi

re: fclean all

cleanrl:
	@$(MAKE) clean -sC $(READLINE_ROOT)

# Regla vacía para forzar la re-evaluación de librerías externas
force:

.PHONY: all clean fclean re cleanrl force
