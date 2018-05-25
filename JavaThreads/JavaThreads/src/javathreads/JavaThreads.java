/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javathreads;

import java.io.File;
import javax.swing.JOptionPane;

/**
 *
 * @author francisco.vicenzi
 */
public class JavaThreads {
    public static void main(String[] args) {
        String busca = JOptionPane.showInputDialog("Digite o programa a ser buscado: ");
        File diretorio = new File("/home/100000000851254/Desktop");
        MyThread thread = new MyThread(busca, diretorio);
        thread.start();
    }
    
}
