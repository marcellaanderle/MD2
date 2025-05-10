#include <stdio.h>
#include <stdlib.h>


long long mdc(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


int eh_primo(long long n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}


long long inverso_modular(long long a, long long n) {
    long long t = 0, novo_t = 1;
    long long r = n, novo_r = a;
    
    while (novo_r != 0) {
        long long quociente = r / novo_r;
        
        long long temp_t = t;
        t = novo_t;
        novo_t = temp_t - quociente * novo_t;
        
        long long temp_r = r;
        r = novo_r;
        novo_r = temp_r - quociente * novo_r;
    }
    
    if (r > 1) return -1;
    if (t < 0) t += n;
    
    return t;
}


long long totient_euler(long long n) {
    if (n == 1) return 1;
    if (eh_primo(n)) return n - 1;
    
    long long resultado = n;
    
    if (n % 2 == 0) {
        resultado -= resultado / 2;
        while (n % 2 == 0) n /= 2;
    }
    
    for (long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            resultado -= resultado / i;
            while (n % i == 0) n /= i;
        }
    }
    
    if (n > 1) resultado -= resultado / n;
    
    return resultado;
}


long long exp_modular(long long a, long long b, long long m) {
    long long resultado = 1;
    a = a % m;
    
    while (b > 0) {
        if (b % 2 == 1) {
            resultado = (resultado * a) % m;
        }
        a = (a * a) % m;
        b = b / 2;
    }
    
    return resultado;
}

int main() {
    long long H, G, n, x, n1;
    
    printf("Digite H, G e n (para calcular a = H / G em Zn): ");
    scanf("%lld %lld %lld", &H, &G, &n);
    
    printf("Digite o expoente x: ");
    scanf("%lld", &x);
    
    printf("Digite o modulo n1: ");
    scanf("%lld", &n1);
    
    printf("\n=== Parte 1: Verificar se G e n sao coprimos ===\n");
    long long gcd_Gn = mdc(G, n);
    if (gcd_Gn != 1) {
        printf("G (%lld) e n (%lld) nao sao coprimos (MDC = %lld).\n", G, n, gcd_Gn);
        printf("A divisao modular nao e possivel pois G nao tem inverso em Zn.\n");
        return 0;
    } else {
        printf("G (%lld) e n (%lld) sao coprimos (MDC = 1).\n", G, n);
    }
    
    printf("\n=== Parte 2: Calcular o inverso de G em Zn ===\n");
    long long inv_g = inverso_modular(G, n);
    if (inv_g == -1) {
        printf("Nao existe inverso para G (%lld) em Z%lld.\n", G, n);
        return 0;
    } else {
        printf("O inverso de G (%lld) em Z%lld e: %lld\n", G, n, inv_g);
    }
    
    printf("\n=== Parte 3: Calcular a = H / G = H * G⁻¹ mod n ===\n");
    long long a = (H % n) * inv_g % n;
    printf("a = (H mod n) * inv_g mod n = (%lld mod %lld) * %lld mod %lld = %lld\n", H, n, inv_g, n, a);
    
    printf("\n=== Parte 4: Verificar se a e n1 sao coprimos ===\n");
    long long gcd_an1 = mdc(a, n1);
    if (gcd_an1 != 1) {
        printf("a (%lld) e n1 (%lld) nao sao coprimos (MDC = %lld).\n", a, n1, gcd_an1);
    } else {
        printf("a (%lld) e n1 (%lld) sao coprimos (MDC = 1).\n", a, n1);
    }
    
    printf("\n=== Parte 5: Verificar se n1 e primo ===\n");
    int n1_primo = eh_primo(n1);
    if (n1_primo) {
        printf("n1 (%lld) e primo.\n", n1);
    } else {
        printf("n1 (%lld) nao e primo.\n", n1);
    }
    
    printf("\n=== Parte 6 e 7: Calcular x1 (o|(n1) ou n1-1) ===\n");
    long long x1;
    if (n1_primo) {
        x1 = n1 - 1;
        printf("Como n1 e primo, aplicamos o Pequeno Teorema de Fermat: x1 = n1 - 1 = %lld\n", x1);
    } else {
        x1 = totient_euler(n1);
        printf("Como n1 nao e primo, aplicamos o Teorema de Euler: x1 = φ(n1) = %lld\n", x1);
    }
    
    printf("\n=== Parte 8: Decompor o expoente x na forma x = x1 * q + r ===\n");
    long long q = x / x1;
    long long r = x % x1;
    printf("x = x1 * q + r => %lld = %lld * %lld + %lld\n", x, x1, q, r);
    
    printf("\n=== Parte 9 e 10: Calcular os valores intermediarios ===\n");
    printf("Expressao: a^x mod n1 ≡ (((a^x1)^q mod n1) * (a^r mod n1)) mod n1\n");
    
    long long x2 = exp_modular(a, x1, n1);
    printf("a^x1 mod n1 = %lld^%lld mod %lld = %lld (x2)\n", a, x1, n1, x2);
    
    long long x2_q = exp_modular(x2, q, n1);
    printf("x2^q mod n1 = %lld^%lld mod %lld = %lld\n", x2, q, n1, x2_q);
    
    long long a_r = exp_modular(a, r, n1);
    printf("a^r mod n1 = %lld^%lld mod %lld = %lld\n", a, r, n1, a_r);
    
    printf("\n=== Parte 11: Combinando os resultados e imprimindo o valor final da congruência ===\n");
    long long resultado = (x2_q * a_r) % n1;
    printf("Resultado final: (%lld * %lld) mod %lld = %lld\n", x2_q, a_r, n1, resultado);
    
    printf("\n=== RESULTADO ===\n");
    printf("%lld^%lld mod %lld e congruente a %lld\n", a, x, n1, resultado);
    
    return 0;
}
