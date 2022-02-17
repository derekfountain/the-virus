;; Shake-the-virus, a ZX Spectrum game
;; Copyright (C) 2022 Derek Fountain
;; 
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;; 
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;; 
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
        
SECTION DATA

;;      ______          _
;;     |  ____|        | |
;;     | |__ ___  _ __ | |_
;;     |  __/ _ \| '_ \| __|
;;     | | | (_) | | | | |_
;;     |_|  \___/|_| |_|\__|
;;

;; The font is called "Mega 1", and is presented by Einar Saukas‎:
;;  https://sites.google.com/site/zxgraph/home/einar-saukas/fonts
;;
;; It's not clear who owns it. I took it from the ZX-ALFA package:
;;  https://spectrumcomputing.co.uk/index.php?cat=96&id=0025283
;; which claims it's sourced from commercial games.
;;
;; Well, Einar, or whoever owns it, thanks! :)

PUBLIC _font

._font

   BINARY "font.fnt"
