module Main where

import           FFICXX.Generate.Builder
import           FFICXX.Generate.Type.Class
import           FFICXX.Generate.Type.Module
import           FFICXX.Generate.Type.PackageInterface

cabal :: Cabal
cabal = Cabal "Nix" "Nix" "Nix"

cabalAttr :: CabalAttr
cabalAttr = CabalAttr
            { cabalattr_license          = Just "BSD3"
            , cabalattr_licensefile      = Just "LICENSE"
            , cabalattr_extraincludedirs = []
            , cabalattr_extralibdirs     = []
            , cabalattr_extrafiles       = []
            }

mkClass :: String -> [Class] -> ProtectedMethod -> Maybe String -> [Function]
        -> Class
mkClass                                  = Class cabal

-- t_vector :: TemplateClass
-- t_vector = TmplCls cabal "Vector" "std::vector" "t"
--            [ TFunNew []
--            , TFun void_ "push_back" "push_back" [(TemplateParam "t", "x")] Nothing
--            , TFun void_ "pop_back" "pop_back" [] Nothing
--            , TFun (TemplateParam "t") "at" "at" [int "n"] Nothing
--            , TFun int_ "size" "size" [] Nothing
--            , TFunDelete
--            ]

tbool :: Types
tbool = CT CTBool NoConst

c_string :: Class
c_string
  = mkClass "string" [] mempty (Just "StdString")
    [
    ]

c_DrvName :: Class
c_DrvName
  = mkClass "DrvName" [] mempty Nothing
    [ Constructor [ (CPT (CPTClassRef c_string) Const, "s") ] Nothing
    , NonVirtual tbool "matches"
      [(CPT (CPTClassRef c_DrvName) NoConst, "n")]
      Nothing
    ]

classes = [c_string, c_DrvName]

templates = []

topLevelFunctions = []

headerMap = [("DrvName", ([NS "nix"], [HdrName "nix/names.hh"]))]

extraDep = []

main :: IO ()
main = simpleBuilder "Nix" headerMap
       (cabal, cabalAttr, classes, topLevelFunctions, templates)
       [] extraDep
