NAME		:= Screensaver
CC			:= g++
CFLAGS		:= -Wall Wextra 
HEADERS		:= -I./includes
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
OS			:= -lglfw -ldl -pthread -l$(OS) m -L/usr/lib64 -lGLEW -lGLU -lGL 

# **************************************************************************** #

# ifdef LINUX #Compile with LINUX=1
# 	OS += -lglfw -ldl -pthread -lm -L/usr/lib64 -lGLEW -lGL -lX11 -lGLU -lOpenGL
# endif
#
# ifndef LINUX
# 	OS += -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit
# endif

all: mkdir $(NAME)

mkdir: 
	@mkdir -p $(OBJDIR)

$(NAME): $(OBJS)
	@$(CC) $^ $(OS) $(HEADERS) $(HPP) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) $(HEADERS) -o $@ -c $< 

run: all 

debug: $(OBJS)
	@$(CC) $^ $(OS) $(HEADERS) $(HPP) -g -fsanitize=address -o $(NAME)

clean:
	@rm -rf $(OBJDIR)

fclean:	clean
	@rm -f $(NAME)

re:	fclean all

.PHONY:	all, clean, fclean, re
