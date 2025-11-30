#!/usr/bin/env python3
import sys
import re

class ImpressoraVM:
    def __init__(self):
        self.stack = []
        self.memory = [0] * 256
        self.registers = {'R0': 0}
        self.pc = 0
        self.instructions = []
        self.labels = {}

        # Estado da impressora
        self.ink_level = 80
        self.paper_count = 500
        self.is_ready = 1
        self.pages_printed = 0
        self.is_color = 0
        self.queue_size = 0
        self.quality = 1  # 0=BAIXA, 1=MEDIA, 2=ALTA
        self.paper_type = 0  # 0=A4, 1=CARTA, 2=A3

    def load_program(self, filename):
        with open(filename, 'r') as f:
            lines = f.readlines()

        # Primeira passada: coletar labels
        line_num = 0
        for line in lines:
            line = line.strip()
            if not line or line.startswith(';'):
                continue

            # Verificar se é label
            if line.endswith(':'):
                label_name = line[:-1]
                self.labels[label_name] = line_num
            else:
                self.instructions.append(line)
                line_num += 1

    def push(self, val):
        self.stack.append(val)

    def pop(self):
        if not self.stack:
            raise RuntimeError("Stack underflow!")
        return self.stack.pop()

    def execute(self):
        while self.pc < len(self.instructions):
            inst = self.instructions[self.pc].strip()

            if not inst:
                self.pc += 1
                continue

            parts = inst.split(None, 1)
            opcode = parts[0]
            operand = parts[1] if len(parts) > 1 else None

            if opcode == 'PUSH':
                self.push(int(operand))

            elif opcode == 'POP':
                val = self.pop()
                if operand in self.registers:
                    self.registers[operand] = val

            elif opcode == 'LOAD':
                addr = int(operand)
                self.push(self.memory[addr])

            elif opcode == 'STORE':
                addr = int(operand)
                self.memory[addr] = self.pop()

            elif opcode == 'ADD':
                b = self.pop()
                a = self.pop()
                self.push(a + b)

            elif opcode == 'SUB':
                b = self.pop()
                a = self.pop()
                self.push(a - b)

            elif opcode == 'MUL':
                b = self.pop()
                a = self.pop()
                self.push(a * b)

            elif opcode == 'DIV':
                b = self.pop()
                a = self.pop()
                self.push(a // b if b != 0 else 0)

            elif opcode == 'NEG':
                self.push(-self.pop())

            elif opcode == 'EQ':
                b = self.pop()
                a = self.pop()
                self.push(1 if a == b else 0)

            elif opcode == 'NE':
                b = self.pop()
                a = self.pop()
                self.push(1 if a != b else 0)

            elif opcode == 'LT':
                b = self.pop()
                a = self.pop()
                self.push(1 if a < b else 0)

            elif opcode == 'LE':
                b = self.pop()
                a = self.pop()
                self.push(1 if a <= b else 0)

            elif opcode == 'GT':
                b = self.pop()
                a = self.pop()
                self.push(1 if a > b else 0)

            elif opcode == 'GE':
                b = self.pop()
                a = self.pop()
                self.push(1 if a >= b else 0)

            elif opcode == 'JUMPZ':
                cond = self.pop()
                if cond == 0:
                    label = operand
                    self.pc = self.labels[label]
                    continue

            elif opcode == 'GOTO':
                label = operand
                self.pc = self.labels[label]
                continue

            elif opcode == 'PRINT':
                val = self.pop()
                print(val)

            elif opcode == 'PRINTS':
                # Remove aspas
                text = operand.strip('"')
                print(text)

            # Comandos da impressora
            elif opcode == 'PRINT_DOC':
                match = re.match(r'"([^"]+)"\s+(\w+)', operand)
                if match:
                    filename = match.group(1)
                    reg = match.group(2)
                    copies = self.registers[reg]

                    print(f"[IMPRESSORA] Imprimindo {copies} cópias de '{filename}'")

                    # Simular impressão
                    if self.ink_level < 10:
                        print("[IMPRESSORA] ERRO: Tinta insuficiente!")
                    elif self.paper_count < copies:
                        print("[IMPRESSORA] ERRO: Papel insuficiente!")
                    else:
                        self.ink_level -= copies * 2
                        self.paper_count -= copies
                        self.pages_printed += copies
                        print(f"[IMPRESSORA] Impressão concluída! ({copies} páginas)")

            elif opcode == 'PRINT_PAGE':
                text = operand.strip('"')
                print(f"[IMPRESSORA] Imprimindo página: {text}")
                self.ink_level -= 1
                self.paper_count -= 1
                self.pages_printed += 1

            elif opcode == 'SET_COLOR':
                reg = operand
                self.is_color = self.registers[reg]
                mode = "COLORIDO" if self.is_color else "P&B"
                print(f"[IMPRESSORA] Modo definido: {mode}")

            elif opcode == 'SET_QUALITY':
                reg = operand
                self.quality = self.registers[reg]
                quality_names = ["BAIXA", "MÉDIA", "ALTA"]
                print(f"[IMPRESSORA] Qualidade definida: {quality_names[self.quality]}")

            elif opcode == 'SET_PAPER':
                reg = operand
                self.paper_type = self.registers[reg]
                paper_names = ["A4", "CARTA", "A3"]
                print(f"[IMPRESSORA] Papel definido: {paper_names[self.paper_type]}")

            elif opcode == 'CHECK_INK':
                print(f"[IMPRESSORA] Nível de tinta: {self.ink_level}%")

            elif opcode == 'CHECK_PAPER':
                print(f"[IMPRESSORA] Folhas disponíveis: {self.paper_count}")

            elif opcode == 'WAIT_READY':
                print("[IMPRESSORA] Aguardando impressora ficar pronta...")
                self.is_ready = 1
                print("[IMPRESSORA] Impressora pronta!")

            elif opcode == 'RESET':
                print("[IMPRESSORA] Resetando contador de páginas...")
                self.pages_printed = 0

            # Sensores
            elif opcode == 'GET_INK_LEVEL':
                self.push(self.ink_level)

            elif opcode == 'GET_PAPER_COUNT':
                self.push(self.paper_count)

            elif opcode == 'GET_IS_READY':
                self.push(self.is_ready)

            elif opcode == 'GET_PAGES_PRINTED':
                self.push(self.pages_printed)

            elif opcode == 'GET_IS_COLOR':
                self.push(self.is_color)

            elif opcode == 'GET_QUEUE_SIZE':
                self.push(self.queue_size)

            elif opcode == 'HALT':
                break

            else:
                print(f"[VM] Instrução desconhecida: {opcode}")

            self.pc += 1

        print("\n[VM] Execução finalizada.")
        print(f"[VM] Estado final da impressora:")
        print(f"  - Tinta: {self.ink_level}%")
        print(f"  - Papel: {self.paper_count} folhas")
        print(f"  - Páginas impressas: {self.pages_printed}")

def main():
    if len(sys.argv) < 2:
        print("Uso: python3 impressoravm.py <arquivo.asm>")
        sys.exit(1)

    vm = ImpressoraVM()
    vm.load_program(sys.argv[1])
    vm.execute()

if __name__ == '__main__':
    main()
