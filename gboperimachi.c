#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#define BUFFER_SIZE 4096 

int main(int argc, char *argv[]) {
    json_error_t error;
    json_t *root = json_load_file("Opcodes.json", JSON_DECODE_ANY, &error);

    if (!root) {
        fprintf(stderr, "Error loading JSON: %s\n", error.text);
        return 1;
    }

    json_t *unprefixed_instructions = json_object_get(root, "unprefixed");
    if (!unprefixed_instructions) {
        fprintf(stderr, "Error: 'Unprefixed instructions' not found in JSON\n");
        json_decref(root);
        return 1;
    }

    FILE *fp = fopen("unprefixed_instructions.rs", "w");
    if (!fp) {
        fprintf(stderr, "Error: cannot open 'unprefixed_instruction.rs'\n");
        json_decref(root);
        return 1;
    }

    fprintf(fp, "impl Cpu {\n");
    fprintf(fp, "    fn execute_instruction(&mut self, opcode: u16) {\n");
    fprintf(fp, "        match opcode {\n");

  const char *opcode_str;
  json_t *instruction_obj;
  json_object_foreach(unprefixed_instructions, opcode_str, instruction_obj) {
    if (!json_is_object(instruction_obj)) {
      fprintf(stderr, "Error: missing for incorrect type for mnemonic, bytes, or cycles in opcode '%s'\n", opcode_str);
      return 1;
    }

    json_t *mnemonic_json = json_object_get(instruction_obj, "mnemonic");
    json_t *bytes_json = json_object_get(instruction_obj, "bytes");
    json_t *cycles_json = json_object_get(instruction_obj, "cycles"); 

    const char *mnemonic = json_string_value(mnemonic_json);
    int bytes = json_integer_value(bytes_json);
    int opcode = strtol(opcode_str + 2, NULL, 16);

    fprintf(fp, "       0x%02X => self.%s(bus, operand),\n", opcode, mnemonic);
  }

  json_decref(root);
  fclose(fp);
  return 0;
}

