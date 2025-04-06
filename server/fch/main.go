package main

import (
	"fmt"
	"net/http"
)


func  getTests (w http.ResponseWriter, r *http.Request) {
  fmt.Println(r.Body)

}

func main() {
  err := http.ListenAndServe("localhost: 1327", getTests)

}
