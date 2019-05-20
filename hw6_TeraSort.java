import java.util.ArrayList;
import java.io.*;
import java.nio.file.Files;
import java.util.Comparator;
import java.util.stream.Stream;
import java.nio.file.Paths;

    class MergeSortFiles extends Thread {
        String f1, f2, f3;
        MergeSortFiles(String f1, String f2, String f3) {
            this.f1 = f1;
            this.f2 = f2;
            this.f3 = f3;
        }

        public void run() {
            try {
                System.out.println(f1 + " Started Merging " + f2 );
                FileReader fR1 = new FileReader(f1);
                FileReader fR2 = new FileReader(f2);
                FileWriter writer = new FileWriter(f3);
                BufferedReader bR1 = new BufferedReader(fR1);
                BufferedReader bR2 = new BufferedReader(fR2);
                String line1 = bR1.readLine();
                String line2 = bR2.readLine();
                
                while (line1 != null || line2 != null) {
                    if (line1 == null || (line2 != null && line1.compareTo(line2) > 0)) {
                        writer.write(line2 + "\r\n");
                        line2 = bR2.readLine();
                    } else {
                        writer.write(line1 + "\r\n");
                        line1 = bR1.readLine();
                    }
                }
                System.out.println(f1 + " Done Merging " + f2 );
                new File(f1).delete();
                new File(f2).delete();
                writer.close();
            } catch (Exception e) {
                System.out.println(e);
            }
        }
    }

    class FileSplit extends Thread {
        String file_name;
        int start_line, end_line;

        FileSplit(String file_name, int start_line, int end_line) {
            this.file_name = file_name;
            this.start_line = start_line;
            this.end_line = end_line;
        }

        public void run() {
            try {
                BufferedWriter writer = Files.newBufferedWriter(Paths.get(file_name));
                int totalLines = end_line + 1 - start_line;
                Stream<String> chunks =
                        Files.lines(Paths.get(TeraSort.fPath))
                                .skip(start_line - 1)
                                .limit(totalLines)
                                .sorted(Comparator.naturalOrder());

                chunks.forEach(line -> {
                    writeToFile(writer, line);
                });
                System.out.println(" Done Writing " + Thread.currentThread().getName());
                writer.close();
            } catch (Exception e) {
                System.out.println(e);
            }
        }

        public static void writeToFile(BufferedWriter writer, String line) {
            try {
                writer.write(line + "\r\n");
            } catch (Exception e) {
                throw new RuntimeException(e);
            }
        }

    }

    public class TeraSort {
        
        //public static final String fdir = "/tmp/";
        public static final String fPath = "input_1GB";
        public static final String opLog = "output_1GB";

        public static void main(String[] args) throws Exception{
            long startTime = System.nanoTime();
            int threadCount = 4; // Number of threads
            //int totalLines = 100000000;
            int totalLines = 10000000;
            int linesPerFile = totalLines / threadCount;
            ArrayList<Thread> activeThreads = new ArrayList<Thread>();

            for (int i = 1; i <= threadCount; i++) {
                int start_line = i == 1 ? i : (i - 1) * linesPerFile + 1;
                int end_line = i * linesPerFile;
                FileSplit mapThreads = new FileSplit("op" + i, start_line, end_line);
                activeThreads.add(mapThreads);
                mapThreads.start();
            }
            activeThreads.stream().forEach(t -> {
                try {
                    t.join();
                } catch (Exception e) {
                }
            });

            int treeHeight = (int) (Math.log(threadCount) / Math.log(2));

            for (int i = 0; i < treeHeight; i++) {
                ArrayList<Thread> actvThreads = new ArrayList<Thread>();

               for (int j = 1, itr = 1; j <= threadCount / (i + 1); j += 2, itr++) {
                    int offset = i * 100;
                    String tempFile1 = "op" + (j + offset);
                    String tempFile2 = "op" + ((j + 1) + offset);
                    String opFile = "op" + (itr + ((i + 1) * 100));

                    MergeSortFiles reduceThreads =
                            new MergeSortFiles(tempFile1,tempFile2,opFile);
                    actvThreads.add(reduceThreads);
                    reduceThreads.start();
                }

                actvThreads.stream().forEach(t -> {
                    try {
                        t.join();
                    } catch (Exception e) {
                    }
                });
            }
            long endTime = System.nanoTime();
            double timeTaken = (endTime - startTime)/1e9;
            System.out.println(timeTaken);
            BufferedWriter logFile = new BufferedWriter(new FileWriter(opLog, true));
            logFile.write("Time Taken in seconds:" + timeTaken);
            Runtime.getRuntime().exec("valsort  " + "op" + (treeHeight*100)+1 + " > " + opLog);
            logFile.close();
        }
    }

