import java.util.Scanner;
import java.util.regex.Pattern;

public class SelectionSort {
    public static void selectionsort(Comparable[] vetor) {
        for (int i = 0; i < vetor.length - 1; i++) {
            int idxMin = i;
            for (int j = i + 1; j < vetor.length; j++) {
                if (vetor[j].compareTo(vetor[idxMin]) < 0) {
                    idxMin = j;
                }
            }
            Comparable aux = vetor[i];
            vetor[i] = vetor[idxMin];
            vetor[idxMin] = aux;
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

        selectionsort(vetor);

        for (int i = 0; i < vetor.length; i++) {
            System.out.println(vetor[i]);
        }
    }
}