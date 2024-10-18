import java.util.Scanner;
import java.util.regex.Pattern;

public class MergeSort {

    public static void mergesort(Comparable[] vetor, int inicio, int fim) {
        if (fim - inicio <= 1) {
            return;
        }
        int meio = inicio + (fim - inicio)/2;
        mergesort(vetor, inicio, meio);
        mergesort(vetor, meio, fim);
        merge(vetor, inicio, meio, fim);
    }

    /* Mescla dois subvetores de números inteiros ordenados, delimitados
     * pelos argumentos inicio, meio e fim, de forma
     * a produzir um vetor ordenado, sobre-escrevendo 
     * o vetor original passado como argumento.
     */
    public static void merge(Comparable[] vetor, int inicio, int meio, int fim) {
        Comparable[] retorno = new Comparable[fim - inicio];
        int j = inicio, k = meio;
        for (int i = 0; i < retorno.length; i++) {
            if (j < meio && k < fim) {
                if (vetor[j].compareTo(vetor[k]) <= 0) {
                    retorno[i] = vetor[j];
                    j++;
                } else {
                    retorno[i] = vetor[k];
                    k++;
                }
            } else if (j < meio) {
                retorno[i] = vetor[j];
                j++;
            } else {
                retorno[i] = vetor[k];
                k++;
            }
        }
        for (int i = 0; i < retorno.length; i++) {
            vetor[inicio + i] = retorno[i];
        }
    }

    public static void main(String[] args) {
        var in = new Scanner(System.in);
        /* Ver FORMATO DOS CASOS DE TESTE.txt para detalhes */
        String[] vars = in.nextLine().split(Pattern.quote(" "));
        int tamanho = Integer.parseInt(vars[0].trim());
        String tipoDeDado = vars[1].trim();

        /* O uso de Comparable evita a necessidade de criar funções separadas
         * para String e int, já que ambos implementam essa interface.
         */
        Comparable[] vetor = new Comparable[tamanho];

        if (tipoDeDado.equals("s")) {
            for (int i = 0; i < vetor.length; i++) {
                vetor[i] = in.nextLine().trim();
            }
        } else if (tipoDeDado.equals("i")) {
            for (int i = 0; i < vetor.length; i++) {
                vetor[i] = Integer.parseInt(in.nextLine().trim());
            }
        }

        mergesort(vetor, 0, vetor.length);

        /* Imprime o vetor ordenado */
        for (int i = 0; i < vetor.length; i++) {
            System.out.println(vetor[i]);
        }
    }
}