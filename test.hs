import System.Process
import Control.Monad

toStr = flip replicate '1' 

test a b = do
  mapM_ system [test, std, cmp]
  where
    input = toStr a ++ "0" ++ toStr b
    test = "./turing-project/turing programs/gcd.tm " ++ input ++ "> .testout"
    std = "echo " ++ toStr (gcd a b) ++ " > .stdout"
    cmp = "cmp .stdout .testout"
    
main :: IO ()
main = do
  forM_ [1..100] $ \i -> do
    forM_ [i..100] $ \j -> do
      putStrLn $ "testing " ++ show (i, j)
      test i j
  test 1145 1910
