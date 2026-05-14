OPTIMIZATION		= 0
COMPILER_FIXED_OPTIONS	= -DHT_OPTIMIZATION=$(OPTIMIZATION)



is_greater = $(shell if [ $(1) -gt $(2) ]; then echo 1; else echo 0; fi)
ifeq ($(call is_greater,$(OPTIMIZATION),0),1)

COMPILER_FIXED_OPTIONS	+= -mavx512f

else ifeq ($(call is_greater,$(OPTIMIZATION),2),1)

ASM_SUF			= .s
make_asm_src_path	= $(addprefix $(SRC_DIR),$(addsuffix $(ASM_SUF), $(1)))



ASM_SRC = list_find
make_asm_obj_rule = $(call make_obj_path,$(1)): $(call make_asm_src_path,$(1)) | prepare;	\
	@nasm -o $$@ -f elf64 $$<
$(foreach src, $(ASM_SRC),$(eval $(call make_asm_obj_rule,$(src))))
$(TARGET): $(call make_obj_path,$(ASM_SRC))

endif



SRC		= Hash_table Hash_table_test main

ISOL_CPU_NUM	= 15
RUN_TARGET	= taskset -c $(ISOL_CPU_NUM) ./$(TARGET) $(KEYS_PATH) $(QUERIES_PATH)

COMMON_MAKEFILE = Common_Makefile.mk
include $(COMMON_MAKEFILE)



DATA_DIR	= data/
DATA_SUF	= .txt
make_data_path	= $(addprefix $(DATA_DIR),$(addsuffix $(DATA_SUF), $(1)))

NOISE_TARGET	= $(call make_exec_path,Noise)

NOISE_PATH	= $(call make_data_path,noise)
UNITED_PATH	= $(call make_data_path,united)
KEYS_PATH	= $(call make_data_path,keys)
QUERIES_PATH	= $(call make_data_path,queries)

FIXED_DATA	= $(call make_data_path,common_dict words_alpha)

$(NOISE_TARGET): | prepare
	@gcc -o $(NOISE_TARGET) $(COMPILER_OPTIONS) $(LINKER_OPTIONS) -I$(INC_DIR) $(call make_src_path,Noise)

update_noise: $(NOISE_TARGET)
	@./$(NOISE_TARGET) $(NOISE_CNT) > $(NOISE_PATH)

update_data:
	@$(MAKE) NOISE_CNT=700000 update_noise
	@cat $(FIXED_DATA) $(NOISE_PATH) | sort -u | shuf > $(UNITED_PATH)
	@wc -c < $(UNITED_PATH) | cat - $(UNITED_PATH) > $(KEYS_PATH)

	@$(MAKE) NOISE_CNT=2000000 update_noise
	@temp=$$(mktemp);								\
	shuf -r -n 18000000 $(UNITED_PATH) | cat - $(NOISE_PATH) | shuf > "$$temp";	\
	wc -c < "$$temp" | cat - "$$temp" > $(QUERIES_PATH);				\
	rm "$$temp"



RESULTS_DIR		= results/
make_results_path	= $(addprefix $(RESULTS_DIR),$(1))



.PHONY: hyperfine_report

prepare::
	@mkdir -p $(RESULTS_DIR)

HYPERFINE_WARMUPS	= 1
HYPERFINE_RUNS		= 5
hyperfine_report: $(TARGET)
	@hyperfine --export-json make_results_path(hyperfine_results_$(OPTIMIZATION).json)	\
	--warmup $(HYPERFINE_WARMUPS) --runs $(HYPERFINE_RUNS) "$(RUN_TARGET)"

perf_report: $(TARGET)
	@perf record -o make_results_path(perf_$(OPTIMIZATION).data) $(RUN_TARGET)
	@perf report -i make_results_path(perf_$(OPTIMIZATION).data)

clean::
	@rm -fr	$(NOISE_PATH) $(UNITED_PATH) $(KEYS_PATH) $(QUERIES_PATH) $(RESULTS_DIR)
