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



SRC = Hash_table HT_test main

TARGET	= Test.elf



MY_OPTIONS	=	-DHT_OPTIMIZATION=0 -mavx512f	\
			-pie -fPIE

ifeq ($(ALLOW_CPP), 1)

MY_OPTIONS	+=	-std=c++23

else

MY_OPTIONS	+=	-std=c23

endif

ifeq ($(RELEASE), 0)

MY_OPTIONS	+=	-Og		\
			-ggdb3 -D_DEBUG

WARNINGS	+=	-Wall -Wextra -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wfloat-equal	\
			-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self		\
			-Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types		\
			-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-missing-field-initializers		\
			-Wno-narrowing -Wno-varargs -Wstack-protector -Wlarger-than=8192 -Wstack-usage=8192 -Werror=vla

FEATURES	+=	-fcheck-new -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer	\
			-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

ifeq ($(ALLOW_CPP), 1)

WARNINGS	+=	-Weffc++ -Wc++14-compat -Woverloaded-virtual -Wconditionally-supported -Wctor-dtor-privacy -Wnon-virtual-dtor -Wsign-promo -Wstrict-null-sentinel	\
			-Wsuggest-override -Wno-literal-suffix -Wno-old-style-cast

FEATURES	+=	-fsized-deallocation

endif

else

MY_OPTIONS	+=	-O3		\
			-DNDEBUG

endif

OPTIONS		=	$(MY_OPTIONS) $(WARNINGS) $(FEATURES)



.PHONY: all prepare update_noise update_data test release_test perf_report clean commit

all: $(TARGET)

prepare:
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)

make_obj = $(call make_obj_path, $(1)): $(call make_src_path, $(1)) | prepare;	\
	@gcc $(OPTIONS) -I$(INC_DIR) -c $$< -o $$@

$(foreach src, $(SRC), $(eval $(call make_obj, $(src))))

$(TARGET): $(call make_obj_path, $(SRC))
	@gcc $(OPTIONS) $^ -o $@
	@echo Compilation end



DATA_DIR	= data/
DATA_SUF	= .txt
make_data_path	= $(addprefix $(DATA_DIR), $(addsuffix $(DATA_SUF), $(1)))

NOISE_CNT	?= 700000
NOISE_PATH	?= $(call make_data_path, noise)

KEYS_PATH	?= $(call make_data_path, keys)
QUERIES_PATH	?= $(call make_data_path, queries) # TODO -

FIXED_DATA	?= common_dict words_alpha

Noise.elf:
	@gcc $(OPTIONS) -I$(INC_DIR) $(call make_src_path, Make_noise) -o Noise.elf

update_noise: Noise.elf
	@./Noise.elf $(NOISE_CNT) > $(NOISE_PATH)

update_data:
	@$(MAKE) NOISE_CNT=700000 update_noise
	@cat $(call make_data_path, $(FIXED_DATA)) $(NOISE_PATH) | sort -u | shuf > $(call make_data_path, united_data)
	@wc -c < $(call make_data_path, united_data) | cat - $(call make_data_path, united_data) > $(KEYS_PATH)

	@$(MAKE) NOISE_CNT=2000000 update_noise
	@temp=$$(mktemp);											\
	shuf -r -n 18000000 $(call make_data_path, united_data) | cat - $(NOISE_PATH) | shuf > "$$temp";	\
	wc -c < "$$temp" | cat - "$$temp" > $(QUERIES_PATH);							\
	rm "$$temp"

test: $(TARGET)
	@taskset -c 15 ./$(TARGET) $(KEYS_PATH) $(QUERIES_PATH)

release_test:
	@$(MAKE) -B RELEASE=1 test

perf_report:
	@$(MAKE) -B RELEASE=1
	@perf record taskset -c 15 ./$(TARGET) $(KEYS_PATH) $(QUERIES_PATH)
	@perf report

clean:
	@rm -fr	$(OBJ_DIR) $(DEP_DIR) $(TARGET) Noise.elf $(NOISE_PATH) $(call make_data_path, united_data) $(KEYS_PATH) $(QUERIES_PATH)

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
