package controllers

import (
	"appSec/myApp/auth"
	"appSec/myApp/utils"
	"log"
	"net/http"

	"github.com/gin-gonic/gin"
)

func LoginHandler(c *gin.Context) {
	c.Header("Cache-Control", "no-cache")

	auth.Nonce, _ = utils.GenerateNonce()
	var redirectPath string

	q := c.Request.URL.Query()
	q.Add("client_id", auth.ClientID)
	q.Add("response_type", "code")
	q.Add("response_mode", "query")
	q.Add("scope", "openid profile email")
	q.Add("redirect_uri", "http://localhost:3000/auth/callback")
	q.Add("state", auth.State)
	q.Add("nonce", auth.Nonce)

	redirectPath = auth.Issuer + "/v1/authorize?" + q.Encode()

	c.Redirect(http.StatusFound, redirectPath)
}

func LogoutHandler(c *gin.Context) {
	if session, err := auth.SessionStore.Get(c.Request, "okta-hosted-login-session-store"); err == nil {
		session.Options.MaxAge = -1
		session.Save(c.Request, c.Writer)
	}

	c.Redirect(http.StatusFound, "/")
}

func AuthCodeCallbackHandler(c *gin.Context) {
	q := c.Request.URL.Query()

	// Check the state that was returned in the query string is the same as the above state
	if q.Get("state") != auth.State {
		log.Println("The state was not as expected")
		c.AbortWithStatus(http.StatusBadRequest)
		return
	}

	// Make sure the code was provided
	if q.Get("code") == "" {
		log.Println("The code was not returned or is not accessible")
		c.AbortWithStatus(http.StatusBadRequest)
		return
	}

	exchange := auth.ExchangeCode(q.Get("code"), c)

	if exchange.Error != "" {
		log.Println(exchange.Error)
		log.Println(exchange.ErrorDescription)
		return
	}

	session, err := auth.SessionStore.Get(c.Request, "okta-hosted-login-session-store")

	if err != nil {
		c.AbortWithStatus(http.StatusInternalServerError)
		return
	}

	_, verificationError := auth.VerifyToken(exchange.IdToken)

	if verificationError != nil {
		log.Println(verificationError)
		c.AbortWithStatus(http.StatusInternalServerError)
		return
	}

	session.Values["id_token"] = exchange.IdToken
	session.Values["access_token"] = exchange.AccessToken
	session.Save(c.Request, c.Writer)
	c.Redirect(http.StatusFound, "/")
}
