package auth

import (
	"appSec/myApp/env"
	"appSec/myApp/model"
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
	SessionStore     = sessions.NewCookieStore([]byte("okta-custom-login-session-store"))
	State            = generateState()
	Nonce            = "NonceNotSetYet"
	ClientID         = env.Get("OKTA_CLIENT_ID")
	ClientSecret     = env.Get("OKTA_CLIENT_SECRET")
	Issuer           = env.Get("OKTA_ISSUER")
	ClaimsToValidate = map[string]string{
		"aud": "api://default",
		"cid": ClientID,
	}
)

func generateState() string {
	// Generate a random byte array for state paramter
	b := make([]byte, 16)
	rand.Read(b)
	return hex.EncodeToString(b)
}

func Verify(c *gin.Context) error {
	token := c.Request.Header.Get("Authorization")

	if !strings.HasPrefix(token, "Bearer ") {
		log.Print("Bearer token not found")
		return errors.New("Unauthorized")
	}

	token = strings.TrimPrefix(token, "Bearer ")

	verifierSetup := jwtverifier.JwtVerifier{
		Issuer:           Issuer,
		ClaimsToValidate: ClaimsToValidate,
	}

	verifier := verifierSetup.New()
	if _, err := verifier.VerifyAccessToken(token); err != nil {
		log.Print("VerifyAccessToken failed")
		return err
	}

	return nil
}

func ExchangeCode(code string, c *gin.Context) model.Exchange {
	authHeader := base64.StdEncoding.EncodeToString(
		[]byte(ClientID + ":" + ClientSecret))

	q := c.Request.URL.Query()
	q.Add("grant_type", "authorization_code")
	q.Set("code", code)
	q.Add("redirect_uri", "http://localhost:3000/auth/callback")

	url := Issuer + "/v1/token?" + q.Encode()

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

func VerifyToken(t string) (*jwtverifier.Jwt, error) {
	tv := map[string]string{}
	tv["nonce"] = Nonce
	tv["aud"] = ClientID
	jv := jwtverifier.JwtVerifier{
		Issuer:           Issuer,
		ClaimsToValidate: tv,
	}

	result, err := jv.New().VerifyIdToken(t)

	if err != nil {
		return nil, fmt.Errorf("%s", err)
	}

	if result != nil {
		return result, nil
	}

	return nil, fmt.Errorf("token could not be verified: %s", "")
}
