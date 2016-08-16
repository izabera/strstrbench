#!/bin/awk -f
BEGIN { lines = 10000; q = "\"" }
BEGINFILE { delete a; problems[FILENAME] = 1 }
FNR < lines {
  for (i = 1; i <= NF; i++) a[$i] = FNR
  next
}
{
  for (i = 1; i <= NF; i++)
    if ($i ~ /^[A-Za-z]+$/ && !($i in a)) {
      printf "{ %-20s, %-50s }, // line %s\n", q $i q, q FILENAME q, FNR
      delete problems[FILENAME]
      nextfile
    }
}
END { for (f in problems) print "nothing found in", f }
