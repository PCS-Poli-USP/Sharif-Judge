#ifndef IHC_H_INCLUDED
#define IHC_H_INCLUDED

#ifndef IHC_H
#define IHC_H

/**
 * Classe de apoio para interface com usuario.
 * @Version EP1
 */
class IHC {
public:
  /**
   * Informa se a tecla ESC foi pressionada.
   */
  bool escFoiPressionado();

  /**
   * Espera por alguns ms.
   */
  void sleep();
};

#endif // IHC_H
#endif
