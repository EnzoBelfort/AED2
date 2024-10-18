import java.util.Scanner;
import java.util.regex.Pattern;

public class RadixSort {
    /**
     * Realiza o radixsort em um vetor de inteiros.
     * maxCasasDecimais é o número de algarismos do maior número presente. */
    public static void intRadixsort(int[] vetor, int maxCasasDecimais) {
        int casaDecimal = 1;
        for (int i = 0; i < maxCasasDecimais; i++) {
            countingsortPorAlgarismo(vetor, casaDecimal);
            casaDecimal *= 10;
        }
    }

    /**
     * Realiza o countingsort em um vetor de inteiros, considerando apenas
     * o algarismo dado por p, onde casaDecimal = 10**p. */
    private static void countingsortPorAlgarismo(int[] vetor, int casaDecimal) {
        int[] contagem = new int[10];
        int[] ordenado = new int[vetor.length];
        for (int i = 0; i < vetor.length; i++) {
            int algarismo = (vetor[i] / casaDecimal) % 10;
            contagem[algarismo]++;
        }
        for (int i = 1; i < contagem.length; i++) {
            contagem[i] += contagem[i-1];
        }
        for (int i = vetor.length - 1; i >= 0; i--) {
            int algarismo = (vetor[i] / casaDecimal) % 10;
            contagem[algarismo]--;
            ordenado[contagem[algarismo]] = vetor[i];
        }
        for (int i = 0; i < ordenado.length; i++) {
            vetor[i] = ordenado[i];
        }
    }

    /**
     * Realiza o radixsort em um vetor de strings.
     * maxCaracteres é o número máximo de caracteres entre os elementos do vetor. */
    public static void stringRadixSort(String[] vetor, int maxCaracteres) {
        for (int idxCaractere = 0; idxCaractere < maxCaracteres; idxCaractere++) {
            countingSortPorCaractere(vetor, idxCaractere, maxCaracteres);
        }
    }

    /**
     * Realiza o countingsort em um vetor de strings, considerando apenas
     * o caractere dado por idxCaractere, onde idxCaractere=0 representa o último caractere da
     * string preenchida com espaços à sua direita até que seu tamanho seja
     * igual a maxCaracteres, e idxCaractere=maxCaracteres-1 é o primeiro caractere da string. */
    private static void countingSortPorCaractere(String[] vetor, int idxCaractere, int maxCaracteres) {
        int[] contagem = new int[27];
        String[] ordenado = new String[vetor.length];
        for (int i = 0; i < vetor.length; i++) {
            contagem[charNaPosicao(vetor[i], maxCaracteres, idxCaractere)]++;
        }
        for (int i = 1; i < contagem.length; i++) {
            contagem[i] += contagem[i-1];
        }
        for (int i = ordenado.length - 1; i >= 0; i--) {
            int c = charNaPosicao(vetor[i], maxCaracteres, idxCaractere);
            ordenado[contagem[c]-1] = vetor[i];
            contagem[c]--;
        }
        for (int i = 0; i < ordenado.length; i++) {
            vetor[i] = ordenado[i];
        }
    }

    /**
     * Calcula o índice não negativo em que a contagem do
     * caractere string[i] será armazenada no vetor de contagem
     * do algoritmo countingsort para vetores de strings.
     * i=0 representa o último caractere da string preenchida com espaços 
     * à sua direita até que seu tamanho seja igual a max, e i=max-1 é o
     * primeiro caractere da string. Os caracteres são mapeados da seguinte maneira:
     * A-Z: números de 1 a 26
     * Outros caracteres: 0 (zero)
     */
    private static int charNaPosicao(String s, int maxCaracteres, int i) {
        int idx = 0;
        char c;
        int diferencaDeTamanhoAoMaior = maxCaracteres - s.length();
        
        if (i >= diferencaDeTamanhoAoMaior) {
            int indiceAPartirDoFinal = i - diferencaDeTamanhoAoMaior;
            c = s.charAt(s.length() - 1 - indiceAPartirDoFinal);
        } else {
            c = ' ';
        }
        if (c == ' ') {
            idx = 0;
        }
        if (c >= 'A' && c <= 'Z') {
            idx = c - 'A' + 1;
        }
        return idx;
    }

    public static void main(String[] args) {
        /* Ver FORMATO DOS CASOS DE TESTE.txt para detalhes */
        var in = new Scanner(System.in);
        String[] vars = in.nextLine().split(Pattern.quote(" "));
        int tamanho = Integer.parseInt(vars[0].trim());
        String tipoDeDado = vars[1].trim();
        int numeroMaximoDeCaracteresOuAlgarismos = Integer.parseInt(vars[2].trim());

        if (tipoDeDado.equals("s")) {
            String[] vetor = new String[tamanho];
            for (int i = 0; i < vetor.length; i++) {
                vetor[i] = in.nextLine().trim();
            }
            stringRadixSort(vetor, numeroMaximoDeCaracteresOuAlgarismos);
            for (int i = 0; i < vetor.length; i++) {
                System.out.println(vetor[i]);
            }
        } else if (tipoDeDado.equals("i")) {
            int[] vetor = new int[tamanho];
            for (int i = 0; i < vetor.length; i++) {
                vetor[i] = Integer.parseInt(in.nextLine().trim());
            }
            intRadixsort(vetor, numeroMaximoDeCaracteresOuAlgarismos);
            for (int i = 0; i < vetor.length; i++) {
                System.out.println(vetor[i]);
            }
        }
    }
}