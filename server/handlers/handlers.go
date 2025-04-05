package handlers


import (
	"net/http"
  "github.com/go-chi/chi"
	chimiddle "github.com/go-chi/chi/middleware"
	log "github.com/sirupsen/logrus"
)

func Handler (r *chi.Mux) {
  //Global middleware
  r.Use (chimiddle.StripSlashes)
  
  r.Route ("/fetch", func (router chi.Router) {

    router.Post("/localhost:1327", somefn)

  })



}
