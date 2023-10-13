naotenhoGuardaChuva.
estaChovendo.

+naoPegarChuva : estaChovendo & naotenhoGuardaChuva
  <- !ficarEmCasa     !estudar.

+naoPegarChuva : not estaChovendo
  <- !sair     !jogarBola.

+comprarGuardaChuva : estaChovendo & naotenhoGuardaChuva
  <- !sair     !procurarLoja     !comprarGuardaChuva.

