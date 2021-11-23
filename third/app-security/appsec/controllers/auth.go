package controllers

import (
	"appSec/myApp/env"
	"appSec/myApp/model"
	"appSec/myApp/utils"
	"bytes"
	"crypto/rand"
	"encoding/base64"
	"encoding/hex"
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"strings"

	"github.com/gin-gonic/gin"
	"github.com/gorilla/sessions"
	jwtverifier "github.com/okta/okta-jwt-verifier-golang"
)

var (
	sessionStore = sessions.NewCookieStore([]byte("okta-custom-login-session-store"))
	state        = generateState()
	nonce        = "NonceNotSetYet"
)

func generateState() string {
	// Generate a random byte array for state paramter
	b := make([]byte, 16)
	rand.Read(b)
	return hex.EncodeToString(b)
}

var toValidate = map[string]string{
	"aud": "api://default",
	"cid": env.Get("OKTA_CLIENT_ID"),
}

func verify(c *gin.Context) error {
	token := c.Request.Header.Get("Authorization")

	if !strings.HasPrefix(token, "Bearer ") {
		log.Print("Bearer token not found")
		return errors.New("Unauthorized")
	}

	token = strings.TrimPrefix(token, "Bearer ")

	verifierSetup := jwtverifier.JwtVerifier{
		Issuer:           env.Get("OKTA_ISSUER"),
		ClaimsToValidate: toValidate,
	}

	verifier := verifierSetup.New()
	if _, err := verifier.VerifyAccessToken(token); err != nil {
		log.Print("VerifyAccessToken failed")
		return err
	}

	return nil
}

func Login(c *gin.Context) {
	c.Header("Cache-Control", "no-cache")

	nonce, _ = utils.GenerateNonce()
	var redirectPath string

	q := c.Request.URL.Query()
	q.Add("client_id", env.Get("OKTA_CLIENT_ID"))
	q.Add("response_type", "code")
	q.Add("response_mode", "query")
	q.Add("scope", "openid profile email")
	q.Add("redirect_uri", "http://localhost:3000/auth/callback")
	q.Add("state", state)
	q.Add("nonce", nonce)

	redirectPath = env.Get("OKTA_ISSUER") + "/v1/authorize?" + q.Encode()

	c.Redirect(http.StatusFound, redirectPath)
}

func AuthCodeCallbackHandler(c *gin.Context) {
	q := c.Request.URL.Query()

	// Check the state that was returned in the query string is the same as the above state
	if q.Get("state") != state {
		log.Println("The state was not as expected")
		return
	}
	// Make sure the code was provided
	if q.Get("code") == "" {
		log.Println("The code was not returned or is not accessible")
		return
	}

	exchange := exchangeCode(q.Get("code"), c)

	if exchange.Error != "" {
		log.Println(exchange.Error)
		log.Println(exchange.ErrorDescription)
		return
	}

	session, err := sessionStore.Get(r, "okta-hosted-login-session-store")
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}

	_, verificationError := verifyToken(exchange.IdToken)

	if verificationError != nil {
		fmt.Println(verificationError)
	}

	if verificationError == nil {
		session.Values["id_token"] = exchange.IdToken
		session.Values["access_token"] = exchange.AccessToken

		session.Save(r, w)
	}

	http.Redirect(w, r, "/", http.StatusFound)
}

func exchangeCode(code string, c *gin.Context) model.Exchange {
	authHeader := base64.StdEncoding.EncodeToString(
		[]byte(env.Get("OKTA_CLIENT_ID") + ":" + env.Get("OKTA_CLIENT_SECRET")))

	q := c.Request.URL.Query()
	q.Add("grant_type", "authorization_code")
	q.Set("code", code)
	q.Add("redirect_uri", "http://localhost:3000/auth/callback")

	url := env.Get("OKTA_ISSUER") + "/v1/token?" + q.Encode()

	req, _ := http.NewRequest("POST", url, bytes.NewReader([]byte("")))
	h := req.Header
	h.Add("Authorization", "Basic "+authHeader)
	h.Add("Accept", "application/json")
	h.Add("Content-Type", "application/x-www-form-urlencoded")
	h.Add("Connection", "close")
	h.Add("Content-Length", "0")

	client := &http.Client{}
	resp, _ := client.Do(req)
	body, _ := ioutil.ReadAll(resp.Body)
	defer resp.Body.Close()
	var exchange model.Exchange
	json.Unmarshal(body, &exchange)

	return exchange
}
