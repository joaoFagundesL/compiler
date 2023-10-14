naotenhoGuardaChuva.
estaChovendo.

!naoPegarChuva.
!comprarGuardaChuva.

@plano1
+!comprarGuardaChuva : estaChovendo & naotenhoGuardaChuva
  <- sair ;
     procurarLoja ;
     comprarGuardaChuva.
