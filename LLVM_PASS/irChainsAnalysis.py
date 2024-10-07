
import argparse
import collections

def count_strings(filename):
  string_counts = collections.defaultdict(int)
  with open(filename, 'r') as file:
    for line in file:
      string = line.strip()
      string_counts[string] += 1

  sorted_counts = dict(sorted(string_counts.items(), key=lambda item: item[1], reverse=True))
  return sorted_counts

def dump_stat(stat):
  for instChain in stat:
    print(str(stat[instChain]) + ": " + instChain)


if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Считывает строки из файла и считает их частоту.')
  parser.add_argument('filename', type=str, help='Имя файла с входными данными.')
  args = parser.parse_args()

  inst_dump = count_strings(args.filename)
  dump_stat(inst_dump)