///////////////////////////////////////////////////////////////
// Program.cs - startup multiple concurrent Clients          //
//                                                           //
///////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.IO;

/////////////////////////////////////////////////////////
// Tester namespace

namespace Tester
{
  /////////////////////////////////////////////////////////
  // Program 

  class Program
  {
    object synch = new object();
    List<string> Results = new List<string>();
    string path = "Debug";

    //----< return List of test results >------------------------------

    List<string> getResults() { return Results; }

    //----< is this the right directory? >-----------------------------

    void showFilesOnPath()
    {
      Console.Write("\n  {0}", Directory.GetCurrentDirectory());
      string[] files = Directory.GetFiles(path);
      foreach (string file in files)
        Console.Write("\n  {0}", file);
      Console.Write("\n");
    }
    //----< Run RequestProcessor showing its window >------------------

    void startTestInWindow()
    {
      ProcessStartInfo pInfo = new ProcessStartInfo();
      pInfo.FileName = path + "\\Client.exe";
      pInfo.Arguments = "1 1 SearchString HREF";
      Process rpProcess = Process.Start(pInfo);
      rpProcess.WaitForExit();
      rpProcess.Dispose();
    }
    //----< Run RequestProcessor windowless >--------------------------

    void startTestNoWindow()
    {
      ProcessStartInfo pInfo = new ProcessStartInfo();
      pInfo.FileName = path + "\\Client.exe";
      pInfo.Arguments = "1 1 SearchString HREF";
      pInfo.UseShellExecute = false;
      pInfo.CreateNoWindow = true;
      pInfo.RedirectStandardOutput = true;
      using (Process rpProcess = Process.Start(pInfo))
      {
        using (StreamReader rdr = rpProcess.StandardOutput)
        {
          while(true)
          {
            if (rdr.EndOfStream)
              break;
            string result = rdr.ReadToEnd();
            lock(synch)
            {
              Results.Add(result);
            }
            //Console.Write("\n  {0}", result);
          }
        }
      }
    }

    //----< process program inputs >--------------

    void processing(string[] args)
    {
      Console.Write("Tester running Client\n");
      Console.Write("======================\n\n");

      //startTestInWindow();

      if(args.Count() == 0)
      {
        Console.Write("Usage: Tester <num_jobs>\n\n");
        return;
      }

      // run specified number of RequestProcessors

      int numToStart = Int32.Parse(args[0]);
      List<Task> taskList = new List<Task>();
      for (int i = 0; i < numToStart; ++i )
      {
        taskList.Add(Task.Run(() => startTestNoWindow()));
      }

      // make sure we have all the results

      foreach (Task task in taskList)
        task.Wait();
      
      // show the results

      List<string> Results = getResults();
      foreach (string result in Results)
        Console.Write(result);

      Console.Write("\n\n *** Test complete *** \n\n");
    }

    //----< Run Tests >------------------------------------------------

    static void Main(string[] args)
    {
      Program tester = new Program();
      tester.processing(args);
    }
  }
}
