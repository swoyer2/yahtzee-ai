import subprocess

for i in range(100):
    result = subprocess.run(['./main'], capture_output=True, text=True, check=True)
    
    with open(f"../v4/sim_{i}.txt", "w") as f:
        f.write(result.stdout.strip())


