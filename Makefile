RELEASE		?= 0
# TODO - Add debug and release directories
OPTIMIZATION	?= 3

ALLOW_CPP	= 1



MY_OPTIONS =	-DHT_OPTIMIZATION=$(OPTIMIZATION) -mavx512f	\
		-pie -fPIE

ifeq ($(ALLOW_CPP),1)

MY_OPTIONS	+=	-std=c++23

else

MY_OPTIONS	+=	-std=c23

endif

ifeq ($(RELEASE),0)

MY_OPTIONS	+=	-Og		\
			-ggdb3 -D_DEBUG

WARNINGS	+=	-Wall -Wextra -Waggressive-loop-optimizations -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wempty-body -Wfloat-equal	\
			-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wopenmp-simd -Wpacked -Wpointer-arith -Winit-self		\
			-Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types		\
			-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-missing-field-initializers		\
			-Wno-narrowing -Wno-varargs -Wstack-protector -Wlarger-than=8192 -Wstack-usage=8192 -Werror=vla

FEATURES	+=	-fcheck-new -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer	\
			-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

ifeq ($(ALLOW_CPP),1)

WARNINGS	+=	-Weffc++ -Wc++14-compat -Woverloaded-virtual -Wconditionally-supported -Wctor-dtor-privacy -Wnon-virtual-dtor -Wsign-promo -Wstrict-null-sentinel	\
			-Wsuggest-override -Wno-literal-suffix -Wno-old-style-cast

FEATURES	+=	-fsized-deallocation

endif

else

MY_OPTIONS	+=	-O3		\
			-DNDEBUG

endif

OPTIONS = $(MY_OPTIONS) $(WARNINGS) $(FEATURES)



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

make_obj_rule = $(call make_obj_path, $(1)): $(call make_src_path, $(1)) | prepare;	\
	@gcc $(OPTIONS) -I$(INC_DIR) -c -o $$@ $$<



.PHONY: all prepare update_noise update_data test release_test perf_report clean commit

prepare:
	@mkdir -p $(OBJ_DIR) $(DEP_DIR)



SRC = Hash_table HT_test main
$(foreach src, $(SRC), $(eval $(call make_obj_rule, $(src))))



TARGET = Test.elf

is_greater = $(shell if [ $(1) -gt $(2) ]; then echo 1; else echo 0; fi)

ifeq ($(call is_greater, $(OPTIMIZATION), 2),1)

ASM_SUF			= .s
make_asm_src_path	= $(addprefix $(SRC_DIR), $(addsuffix $(ASM_SUF), $(1)))

make_asm_obj_rule = $(call make_obj_path, $(1)): $(call make_asm_src_path, $(1)) | prepare;	\
	@nasm -f elf64 -o $$@ $$<

ASM_SRC = list_find
$(foreach src, $(ASM_SRC), $(eval $(call make_asm_obj_rule, $(src))))

$(TARGET): $(call make_obj_path, $(SRC) $(ASM_SRC))
	@gcc $(OPTIONS) -o $@ $^
	@echo Compilation end

else

$(TARGET): $(call make_obj_path, $(SRC))
	@gcc $(OPTIONS) -o $@ $^
	@echo Compilation end

endif

.DEFAULT_GOAL = $(TARGET)


DATA_DIR	= data/
DATA_SUF	= .txt
make_data_path	= $(addprefix $(DATA_DIR), $(addsuffix $(DATA_SUF), $(1)))

NOISE_PATH	?= $(call make_data_path, noise)
KEYS_PATH	?= $(call make_data_path, keys)
QUERIES_PATH	?= $(call make_data_path, queries)

FIXED_DATA	?= common_dict words_alpha

Noise.elf:
	@gcc $(OPTIONS) -I$(INC_DIR) -o Noise.elf $(call make_src_path, Make_noise)

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



RUN_TARGET = taskset -c 15 ./$(TARGET) $(KEYS_PATH) $(QUERIES_PATH)

test: $(TARGET)
	@$(RUN_TARGET)

release_test:
	@$(MAKE) -B RELEASE=1 test

perf_report:
	@$(MAKE) -B
	@perf record -o perf_$(OPTIMIZATION)O.data $(RUN_TARGET)
	@perf report -i perf_$(OPTIMIZATION)O.data

clean:
	@rm -fr	$(OBJ_DIR) $(DEP_DIR) $(TARGET) Noise.elf

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
