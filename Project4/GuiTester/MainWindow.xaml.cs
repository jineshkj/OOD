using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.Diagnostics;
using System.IO;

/////////////////////////////////////////////////////////
// Dispatcher

namespace GuiTester
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  /////////////////////////////////////////////////////////
  // MainWindow

  public partial class MainWindow : Window
  {
    //----< MainWindow constructor >--------------

    public MainWindow()
    {
      InitializeComponent();
    }

    //----< handler for button click >--------------

    private void Button_Click(object sender, RoutedEventArgs e)
    {
      ProcessStartInfo pInfo = new ProcessStartInfo();
      pInfo.FileName = "Debug\\Client.exe";
      pInfo.Arguments = NIterations.Text + " " + NThreads.Text + " " + " SearchString \"" + SearchString.Text + "\"";
      pInfo.UseShellExecute = false;
      pInfo.CreateNoWindow = true;
      pInfo.RedirectStandardOutput = true;

      using (Process rpProcess = Process.Start(pInfo))
      {
        using (StreamReader rdr = rpProcess.StandardOutput)
        {
          while (true)
          {
            if (rdr.EndOfStream)
              break;

            ResultsArea.Content = rdr.ReadToEnd();
            ResultsArea.ScrollToEnd();
          }
        }
      }

    }

  }
}
