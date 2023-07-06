NAME		:= Screensaver
CC			:= g++
CFLAGS		:= -Wall -Wextra 
HEADERS		:= -I ./includes 
SRCS		:= 	Application.cpp \
				IndexBuffer.cpp \
				Renderer.cpp \
				VertexBuffer.cpp \

HPP			:= 	IndexBuffer.h \
				Renderer.h \
				VertexBuffer.h \

SRCDIR 		:= ./src
OBJDIR 		:= ./obj
OBJS		:= $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))
SRCS		:= $(addprefix $(SRCDIR)/,$(SRCS))

# **************************************************************************** #

ifdef LINUX #Compile with LINUX=1
	OS += -lglfw -ldl -pthread -lm -L/usr/lib64 -lGLEW -lGL -lX11 -lGLU -lOpenGL
endif

ifndef LINUX
	OS += -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit
endif

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $^ $(OS) $(HEADERS) $(HPP) -o $(NAME)
	# @$(CC) $^ $(OS) $(HEADERS) $(HPP) -g -fsanitize=address -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

clean:
	@rm -rf $(OBJDIR)

fclean:	clean
	@rm -f $(NAME)

re:	fclean all

.PHONY:	all, clean, fclean, re
