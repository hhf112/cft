package main

import (
	"net/http"

	"github.com/go-chi/chi"
	log "github.com/sirupsen/logrus"
)

func main() {
  //logs errors
	log.SetReportCaller(true)

  //a handler thingy, express equivalent .
  var r *chi.Mux = chi.NewRouter();

  //typical http server by go
	err := http.ListenAndServe("localhost:1327", r)
  if err != nil {
    log.Error(err)
  }
  


}
