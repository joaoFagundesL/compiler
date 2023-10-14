naotenhoGuardaChuva.
estaChovendo.

!naoPegarChuva.
!comprarGuardaChuva.

@plano1
+!naoPegarChuva : estaChovendo & naotenhoGuardaChuva
  <- ficarEmCasa ;
     estudar.

@plano2
+!naoPegarChuva : not estaChovendo
  <- sair ;
     jogarBola.

@plano3
+!comprarGuardaChuva : estaChovendo & naotenhoGuardaChuva
  <- sair ;
     procurarLoja ;
     comprarGuardaChuva.
