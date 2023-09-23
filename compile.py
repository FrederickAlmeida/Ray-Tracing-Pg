import subprocess
import os
from PIL import Image

def compile(cpp_file, compiled_file):
    compile_command = f"g++ {cpp_file} -o {compiled_file}"
    try:
        subprocess.run(compile_command, shell=True, check=True)
        print("Compilação bem-sucedida.")
        return True
    except subprocess.CalledProcessError as e:
        print("Erro na compilação:", e)
        exit(1)

def run(input_file, compiled_file, output_file):
    if os.path.exists(input_file):
        try:
            exec_path = os.path.abspath(compiled_file)
            completed_process = subprocess.run(
                [exec_path],
                stdin=open(input_file, "r"),
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                shell=True,
                text=True
            )

            if completed_process.returncode != 0:
                print("Erro ao executar o programa. Saída de erro:")
                print(completed_process.stderr)
            else:
                output_data = completed_process.stdout
                print("Saída do programa:")

                with open(output_file, "w") as f:
                    f.write(output_data)
                    print(f"A saída foi salva em {output_file}")
                    return True
        except subprocess.CalledProcessError as e:
            print("Erro ao executar o programa:", e)
    else:
        print(f"O arquivo {input_file} não foi encontrado.")

def getPNG(ppm_file):
    try:
        img = Image.open(ppm_file)
        img.save('saida.png', "PNG")
        print(f"Arquivo PPM convertido para imagem e salvo")
    except Exception as e:
        print(f"Erro ao converter a imagem: {e}")

if __name__ == "__main__":
    # Edite se tiver que usar outro arquivo
    if compile('main.cpp', 'main.exe'):
        if run('input.txt', 'main.exe', 'output.ppm'):
            getPNG('output.ppm')
