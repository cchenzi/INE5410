/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javathreads;

import java.io.File;
import java.io.FilenameFilter;

/**
 *
 * @author francisco.vicenzi
 */
public class MyThread extends Thread {

    private final String busca;
    private final File diretorio;
    
    public MyThread(String busca, File diretorio) {
        this.busca = busca;
        this.diretorio = diretorio;
    }
    
    public void run() {        
        File[] encontrados = diretorio.listFiles();
        for (int i = 0; i < encontrados.length; i++) {
            if(encontrados[i].isDirectory()) {
                String aux = encontrados[i].getName();
                aux = diretorio+"/"+aux;
                File novoDiretorio = new File(aux);
                System.out.println("Entrando em novo diretório: " + aux);
                MyThread thread = new MyThread(busca, novoDiretorio);
                thread.start();
            } else {
                if (encontrados[i].getName().startsWith(busca)) {
                    System.out.println("Encontrado em: " + getDiretorio().toString());
                }
            }
        }
          
        
    }
    
    public File getDiretorio() {
        return this.diretorio;
    }
}
