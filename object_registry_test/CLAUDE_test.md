## Test run

Exe: `D:/KADLUB/cvz/APPS/PROMPT_ASSEMBLER/debug/PROMPT_ASSEMBLER.exe`
Commands via stdin (`echo "cmd1\ncmd2" | exe`), output on stdout.

### How to test
1. Send ~10 create/modify commands (mix of create, delete, change)
2. 0-1x `create_model_test` per series
3. Send _llm display commands to verify expected state
4. Default: 1 series per run unless told otherwise

### Output format
1. Numbered list of commands sent: `[1] command ...`
2. Evaluation: verify each container's contents match expected state based on filters and operations
3. Note issues (missing objects, wrong containers, unexpected warnings)

## Commands (object_registry_test)

### Create / modify
- `create_object_test <val>...` — create TestObject(s), multiple values allowed
- `create_model_test <from> <to>` — create TestModel container, returns container_index
- `delete_object_test <id>...` — delete objects by id, multiple ids allowed
- `change_object_test <id> <val>` — change object value (triggers mutableContainment)

### Display (_llm = plain text for LLM parsing)
- `oreg_test_display_pool_llm` — object ids in pool
- `oreg_test_container_display_llm <i>` — container items: id + value per object
- `oreg_list_containers_llm` — container indices
- `oreg_test_object_display <id>` — single object detail (oo_to_string)
- `display_model_col_test` — TestModelCol_Model overview
