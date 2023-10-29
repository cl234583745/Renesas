[File name]
 e2_studio_Macro_for_RZN2L_rev1.0.zip

  rzn2l_xspi0_x1_boot.cfg
   - Please use when using xSPI0 x1 boot mode at 1.8V. Applicable to RSK+RZ/N2L.

[Tool]
 e2 studio 2022-10

[Use of macro]
 Please set up following in e2 studio project.

  1. Place "rzn2l_xspi0_x1_boot.cfg" directly under the project.
  2. Open "Debug Configurations" and select the project from "Renesas GDB Hardware Debugging".
  3. Click on "Startup" tab and add "source rzn2l_xspi0_x1_boot.cfg" to "Run Commands" field.
  
 For detailed, please refer to "RZ/T2M, RZ/N2L Getting Started with Flexible Software Package (r01an6434ej)".

[Caution]
 Do not apply this macro file except in xSPI0 x1 boot mode.

[History]

 Rev 1.0  | Dec 23, 2022 | Initial release
