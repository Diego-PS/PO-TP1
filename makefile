# Copyright 2018 Matheus Nunes <mhnnunes@dcc.ufmg.br>
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

INCLUDE_FOLDER = ./include/
BIN_FOLDER = ./
OBJ_FOLDER = ./obj/
SRC_FOLDER = ./src/

CC = g++

# The flags below will be included in the implicit compilation rules
# 'make' infers that the code in the directory is in c++
# and looks for the CPPFLAGS variable
CPPFLAGS = -g -Wall -std=c++11 -O3

MAIN = main
TARGET = tp1.out
SRC = $(wildcard $(SRC_FOLDER)*.cpp)
OBJ = $(patsubst $(SRC_FOLDER)%.cpp, $(OBJ_FOLDER)%.o, $(SRC))

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER)

build: $(OBJ)
	$(CC) $(CPPFLAGS) -o $(BIN_FOLDER)$(TARGET) $(OBJ)

clean:
	@rm -rf $(OBJ_FOLDER)* $(BIN_FOLDER)tp1.out