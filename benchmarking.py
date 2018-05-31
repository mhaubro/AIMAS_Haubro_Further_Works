import glob, os, zipfile, datetime, shutil, sys, re

def didSolve(filename):
    with open(filename) as f:
        lines = f.readlines()
        for i in range(0, len(lines)):
            line = lines[i]            
            if 'unsuccessful' in line:
                continue
            if 'successful' in line:
                return [lines[i+1].rstrip("\n\r"), lines[i+2].rstrip("\n\r")]
    return None

def average(list):
    sum = 0
    for n in list:
        sum += n
    return int(sum/len(list))
            
if __name__ == "__main__":
    print("\n\n")
    if len(sys.argv) > 1:
        output = sys.argv[1]
    else:
        output = 'results/NotHardAfterMerge.out'

    path = os.getcwd()
    results = 'results/results.txt'
    tmp = 'results/tmp'

    testResult = []
    tests = []
    testsTotal = 0
    time = []
    actions = []

    with zipfile.ZipFile(os.path.join(path, output),"r") as zip_ref:
        zip_ref.extractall(tmp)

    for f in sorted(glob.glob(os.path.join(path, tmp + '/*.log'))):
        testsTotal += 1
        ret = didSolve(f)
        if(didSolve(f)):
            print("Solved: " + os.path.basename(f).replace(".log", "") + " in " + ret[0] + " milliseconds and " + ret[1] + " actions")
            tests.append(os.path.basename(f).replace(".log", ""))
            time.append(int(ret[0]))
            actions.append(int(ret[1]))
        else:
            print("Could not solve: " + os.path.basename(f).replace(".log", ""))

    print("\n\n")
    print("======================================================")
    print("Summary - solved " +  str(len(tests)) + " out of " + str(testsTotal))
    print("Average of " + str(average(time)) + " milliseconds and " + str(average(actions)) + " actions for solved levels")
    print("======================================================")
    shutil.rmtree(tmp)
