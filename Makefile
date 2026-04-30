ALLOW_CPP	?= 1
RELEASE		?= 0



INC_DIR		= inc/

SRC_DIR		= src/
SRC_SUF		= .cpp
make_src_path	= $(addprefix $(SRC_DIR), $(addsuffix $(SRC_SUF), $(1)))

OBJ_DIR		= bin/
OBJ_SUF		= .o
make_obj_path	= $(addprefix $(OBJ_DIR), $(addsuffix $(OBJ_SUF), $(1)))

DEP_DIR		= dep/
DEP_SUF		= .d
make_dep_path	= $(addprefix $(DEP_DIR), $(addsuffix $(DEP_SUF), $(1)))



SRC	= Hash_table HT_test main
TARGET	= Test.elf



MY_OPTIONS	=	-mavx512f	\
			-pie

ifeq ($(ALLOW_CPP), 1)

MY_OPTIONS	+=	-std=c++23

else

MY_OPTIONS	+=	-std=c23

endif

ifeq ($(RELEASE), 0)

MY_OPTIONS	+=	-Og		\
			-ggdb3 -D_DEBUG

C_WARNINGS	=	-Wall -Wextra -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wfloat-equal	\
			-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self		\
			-Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types		\
			-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-missing-field-initializers		\
			-Wno-narrowing -Wno-varargs -Wstack-protector -Wlarger-than=8192 -Wstack-usage=8192 -Werror=vla

C_FEATURES	=	-fcheck-new -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -fPIE	\
			-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

ifeq ($(ALLOW_CPP), 1)

CPP_WARNINGS	=	-Weffc++ -Wc++14-compat -Woverloaded-virtual -Wconditionally-supported -Wctor-dtor-privacy -Wnon-virtual-dtor -Wsign-promo -Wstrict-null-sentinel	\
			-Wsuggest-override -Wno-literal-suffix -Wno-old-style-cast

CPP_FEATURES	=	-fsized-deallocation

endif

else

MY_OPTIONS	+=	-Ofast		\
			-DNDEBUG

endif

OPTIONS		=	$(MY_OPTIONS) $(C_WARNINGS) $(C_FEATURES) $(CPP_WARNINGS) $(CPP_FEATURES)



.PHONY: all prepare test noise data_base clean commit

all: $(TARGET)

prepare:
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)

$(TARGET): $(call make_obj_path, $(SRC))
	@gcc $(OPTIONS) $^ -o $@
	@echo Compilation end

make_obj = $(call make_obj_path, $(1)): $(call make_src_path, $(1)) | prepare;	\
	@gcc $(OPTIONS) -I$(INC_DIR) -c $$< -o $$@

$(foreach src, $(SRC), $(eval $(call make_obj, $(src))))

test: $(TARGET) | data_base
	@./$(TARGET)

noise:
	@gcc $(OPTIONS) -I$(INC_DIR) $(call make_src_path, Make_noise) -o Noise.elf
	@./Noise.elf

data_base:
	@$(MAKE) noise
	@cat data/Noise_test.txt data/words_alpha.txt | shuf > data/united_data.txt
	@wc -c < data/united_data.txt > data/keys.txt
	@cat data/united_data.txt >> data/keys.txt

clean:
	@rm -fr	$(OBJ_DIR) $(DEP_DIR) $(TARGET)

commit:
	@git add .
	@git commit -m "$(MSG)"
	@git push



.SECONDEXPANSION:

%$(DEP_SUF): $(call make_src_path, $$(*F)) | prepare
	@gcc -MM $(OPTIONS) -I$(INC_DIR) $< | sed 's,$(addsuffix \($(*F)\), $(OBJ_SUF))[ :]*,$(call make_obj_path, \1) $@: ,g' > $@

ifeq ($(filter clean, $(MAKECMDGOALS)),)

include $(call make_dep_path, $(SRC))

endif